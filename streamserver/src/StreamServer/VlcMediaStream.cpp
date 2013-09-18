#include "StdAfx.h"
#include <cassert>
#include "VlcMediaStream.h"
#include "SSExcept.h"
#include "INISet.h"

CVlcMediaStream::CVlcMediaStream(void):m_pinst(NULL), m_pml(NULL), m_pmlp(NULL), m_pmp(NULL)
{
}

CVlcMediaStream::~CVlcMediaStream(void)
{
	libvlc_media_list_player_release(m_pmlp);
	libvlc_media_list_release(m_pml);
	libvlc_release(m_pinst);
}


//播放前需调用此函数完成初始化工作
void CVlcMediaStream::SetVlcCore(const char *ip, const char *port)
{
	string vlc_para = string("--sout=#transcode{vcodec=none,acodec=mp3,ab=128,channels=2,samplerate=44100}:rtp{dst=") + string(ip) + string(",port=") +string(port) + string("}");
	const char * const vlc_args[] = {
		"-I", "dummy", /* Don't use any interface */
		"--random",
		"--ignore-config", /* Don't use VLC's config */
		"--plugin-path=/plugin",
		vlc_para.c_str()};
		//"--sout=#transcode{vcodec=none,acodec=mp3,ab=128,channels=2,samplerate=44100}:rtp{dst=192.168.1.255,port=5004}"};

		if (NULL != m_pinst)
		{
			libvlc_release(m_pinst);
			libvlc_media_list_release(m_pml);
			libvlc_media_list_player_release(m_pmlp);
			libvlc_media_player_release(m_pmp);
			m_pinst = NULL;
			m_pml = NULL;
			m_pmlp = NULL;
		}

		//Load the VLC engine
		m_pinst = libvlc_new (sizeof(vlc_args) / sizeof(vlc_args[0]), vlc_args);

		//Create a media list playing environement
		m_pml = libvlc_media_list_new(m_pinst);

		//Create a media player playing environement
		m_pmlp = libvlc_media_list_player_new(m_pinst);

		//设定一个media_player 'attach' media_list_player
		//这样我们可以通过这个media_player获取正在播放的媒体，从而获得详细信息
		m_pmp = libvlc_media_player_new(m_pinst);
		libvlc_media_list_player_set_media_player(m_pmlp, m_pmp);
}

//开始播放
void CVlcMediaStream::PlayMediaStream()
{
	libvlc_media_list_player_set_media_list(m_pmlp, m_pml);

	//对当前播放列表设置事件管理器，当特定事件发生时调用指定的回调函数
	libvlc_event_manager_t *event = libvlc_media_list_player_event_manager(m_pmlp);

	//当播放到下一首歌曲时，触发回调
	libvlc_event_attach(event, libvlc_MediaListPlayerNextItemSet, OnEventProc, this);

	//可以再增加感兴趣的事件，目前只想知道下一首歌曲的播放时间点
	//To add here ...

	//default for loop
	SetPlaybackMode(libvlc_playback_mode_loop);
	libvlc_media_list_player_play (m_pmlp);
}

//根据路径指定播放列表
//首先释放先前的列表，然后载入新的列表
//适用于服务器重启的情况
void CVlcMediaStream::SetMediaPlayList(vector<string> vstrPath)
{
	//first,relase the media_list older
	libvlc_media_list_release(m_pml);
	m_pml = libvlc_media_list_new(m_pinst);

	for(vector<string>::const_iterator it = vstrPath.begin(); it != vstrPath.end(); ++it)
	{
		libvlc_media_t *pmedia = libvlc_media_new_path(m_pinst, it->c_str());
		int i = libvlc_media_list_add_media(m_pml, pmedia);
		assert(i == 0);
		libvlc_media_release(pmedia);
	}

}


//设定播放模式，默认为循环播放
void CVlcMediaStream::SetPlaybackMode(libvlc_playback_mode_t e_mode)
{
	libvlc_media_list_player_set_playback_mode(m_pmlp, e_mode);

}

//暂停播放
void CVlcMediaStream::PauseMediaPlay()
{
	libvlc_media_list_player_pause(m_pmlp);
}

//根据index删除列表中歌曲
void CVlcMediaStream::DelItemsByIndexes(const vector<int> &vindexs)
{
	for (vector<int>::const_iterator iter = vindexs.begin(); iter != vindexs.end(); ++iter)
	{
		libvlc_media_list_remove_index(m_pml, *iter);
	}
}

//加入指定路径的歌曲到播放列表中
void CVlcMediaStream::AddItemsByPaths(const vector<string> &vstrPaths)
{
	for (vector<string>::const_iterator iter = vstrPaths.begin(); iter != vstrPaths.end(); ++iter)
	{
		libvlc_media_t *pm = libvlc_media_new_path(m_pinst, (*iter).c_str());
		int i = libvlc_media_list_add_media(m_pml, pm);
		assert(i == 0);
		libvlc_media_release(pm);
	}
}

//实现观测者模式，便于外部监听当前的播放状态
void CVlcMediaStream::AddListener(CMediaStateListener *pListener)
{
	assert(NULL != pListener);
	m_vListeners.push_back(pListener);
}


//注销所有监听者
void CVlcMediaStream::DelListener(CMediaStateListener *pListener)
{
	vector<CMediaStateListener *>::iterator iter = m_vListeners.begin();
	for (; iter != m_vListeners.end(); iter++)
	{
		string name = (*iter)->GetName();
		if (0 == name.compare(pListener->GetName()))
		{
			m_vListeners.erase(iter);
		}
	}
}


//通知监听者们
//@para eEvent:事件类型, info:额外信息
void CVlcMediaStream::NotifyListeners(libvlc_event_e eEvent, const string &info)
{
	vector<CMediaStateListener *>::iterator iter = m_vListeners.begin();
	for (; iter != m_vListeners.end(); iter++)
	{
		(*iter)->Update(eEvent, info);
	}
}

//这个回调机制设计比较无奈，因为是调用的Ｃ库，无法把类成员函数传递给形参
//只好用static 去掉this指针，另外把类实例的指针作为形参传给回到函数，以便使用类成员变量
void  CVlcMediaStream::OnEventProc(const struct libvlc_event_t *event, void *pCMediaStreamObj)
{
	assert(NULL != pCMediaStreamObj);

	CVlcMediaStream *pMediaStream = static_cast<CVlcMediaStream *>(pCMediaStreamObj);
	string info;
	switch (event->type)
	{
	//播放下一首歌时
	//info存放歌曲路径
	case libvlc_MediaListPlayerNextItemSet:
		{
			libvlc_media_t *pm = libvlc_media_player_get_media(pMediaStream->m_pmp);
			info = string(libvlc_media_get_mrl(pm));
		}
		break;

	default:
		//没有期望监听的事件发生
		return;
	}
	pMediaStream->NotifyListeners(static_cast<libvlc_event_e>(event->type), info);
}

void CVlcMediaStream::PlayAlert(const string &alert_path)
{
	libvlc_media_t *pm = libvlc_media_new_path(m_pinst, alert_path.c_str());
	m_pAlertMediaList = libvlc_media_list_new(m_pinst);
	libvlc_media_list_add_media(m_pAlertMediaList, pm);
	libvlc_media_release(pm);
	m_pAlertMediaListPlayer = libvlc_media_list_player_new(m_pinst);
	libvlc_media_list_player_set_media_list(m_pAlertMediaListPlayer, m_pAlertMediaList);
	libvlc_media_list_player_set_playback_mode(m_pAlertMediaListPlayer, libvlc_playback_mode_loop);
	libvlc_media_list_player_play(m_pAlertMediaListPlayer);
}

void CVlcMediaStream::PlaySafe()
{
	libvlc_media_list_player_stop(m_pAlertMediaListPlayer);
	libvlc_media_list_release(m_pAlertMediaList);
	libvlc_media_list_player_release(m_pAlertMediaListPlayer);
}

void CVlcMediaStream::PlayAlertPause()
{
	libvlc_media_list_player_pause(m_pAlertMediaListPlayer);
}

void CVlcMediaStream::PlayAlertResume()
{
	libvlc_media_list_player_play(m_pAlertMediaListPlayer);
}

void CVlcMediaStream::PlayMicro()
{
	m_pMicroMedia = libvlc_media_new_location(m_pinst, "dshow://");
	if (NULL == m_pMicroMedia)
	{
		throw CMediaExcept(INIT_AUDIO_INPUT_ERR);
		return;
	}
	m_pMicroMediaPlayer = libvlc_media_player_new_from_media (m_pMicroMedia);
	libvlc_media_player_play(m_pMicroMediaPlayer);
}

void CVlcMediaStream::StopMicro()
{
	libvlc_media_player_stop(m_pMicroMediaPlayer);
	libvlc_media_player_release(m_pMicroMediaPlayer);
	libvlc_media_release(m_pMicroMedia);
}