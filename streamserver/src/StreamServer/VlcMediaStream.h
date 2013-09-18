#pragma once

#include <memory>
#include <vector>
#include <string>

#include <vlc/vlc.h>

#include "libvlcwrapper/VlcCore.h"
#include "libvlcwrapper/Media.h"
#include "libvlcwrapper/MediaList.h"
#include "libvlcwrapper/MediaPlayer.h"
#include "libvlcwrapper/MediaList.h"
#include "libvlcwrapper/MediaListPlayer.h"

#include "MediaStateListener.h"

using namespace std;

class CVlcMediaStream
{
public:
	CVlcMediaStream(void);
	CVlcMediaStream(const char *ip, const char *port);
	~CVlcMediaStream(void);

public:
	void PlayMediaStream(void);
	void PauseMediaPlay(void);
	void SetMediaPlayList(vector<string> vstrPath);
	void SetPlaybackMode(libvlc_playback_mode_t e_mode);
	void DelItemsByIndexes(const vector<int> &vindexs);
	void AddItemsByPaths(const vector<string> &vstrPaths);
	void SetVlcCore(const char *ip, const char *port);

	//注册观测者
	void AddListener(CMediaStateListener *pListener);
	void DelListener(CMediaStateListener *pListener);
	void NotifyListeners(libvlc_event_e eEvent, const string &info);

	//报警对应
	void PlayAlert(const string &alert_path);
	void PlaySafe();
	void PlayAlertPause();
	void PlayAlertResume();

	//micro广播
	void PlayMicro(void);
	void StopMicro(void);

private:
	//此成员函数为回调函数机制而设
	static void OnEventProc(const struct libvlc_event_t *event, void *pCMediaStreamObj);

private:
	libvlc_instance_t *m_pinst;
	libvlc_media_list_player_t *m_pmlp;
	libvlc_media_list_t *m_pml;
	libvlc_media_player_t *m_pmp;
	vector<CMediaStateListener *> m_vListeners;

	//报警时临时播放
	libvlc_media_list_t *m_pAlertMediaList;
	libvlc_media_list_player_t *m_pAlertMediaListPlayer;

	//micro广播
	libvlc_media_t *m_pMicroMedia;
	libvlc_media_player_t *m_pMicroMediaPlayer;

};
