#pragma once

#include <string>
#include <vlc/vlc.h>

using namespace std;


//这是监听类，因为应用比较简单，没有用虚类实现接口的封装，直接写在此类中
//同MFC配合使用时，需要多继承此类，重载Update()函数完成所需功能
class CMediaStateListener
{
public:
	//默认为匿名监听者
	CMediaStateListener(void);

	CMediaStateListener(string strName);
	~CMediaStateListener(void);


public:
	virtual void Update(libvlc_event_e eEvent, const string &info);
	string GetName(void);

public:

	//对外暴露的监听事件类型。同libvlc_event_e完全相同
	enum CListen_Event_event_e {
    CListen_Event_MediaMetaChanged=0,
    CListen_Event_MediaSubItemAdded,
    CListen_Event_MediaDurationChanged,
    CListen_Event_MediaParsedChanged,
    CListen_Event_MediaFreed,
    CListen_Event_MediaStateChanged,

    CListen_Event_MediaPlayerMediaChanged=0x100,
    CListen_Event_MediaPlayerNothingSpecial,
    CListen_Event_MediaPlayerOpening,
    CListen_Event_MediaPlayerBuffering,
    CListen_Event_MediaPlayerPlaying,
    CListen_Event_MediaPlayerPaused,
    CListen_Event_MediaPlayerStopped,
    CListen_Event_MediaPlayerForward,
    CListen_Event_MediaPlayerBackward,
    CListen_Event_MediaPlayerEndReached,
    CListen_Event_MediaPlayerEncounteredError,
    CListen_Event_MediaPlayerTimeChanged,
    CListen_Event_MediaPlayerPositionChanged,
    CListen_Event_MediaPlayerSeekableChanged,
    CListen_Event_MediaPlayerPausableChanged,
    CListen_Event_MediaPlayerTitleChanged,
    CListen_Event_MediaPlayerSnapshotTaken,
    CListen_Event_MediaPlayerLengthChanged,

    CListen_Event_MediaListItemAdded=0x200,
    CListen_Event_MediaListWillAddItem,
    CListen_Event_MediaListItemDeleted,
    CListen_Event_MediaListWillDeleteItem,

    CListen_Event_MediaListViewItemAdded=0x300,
    CListen_Event_MediaListViewWillAddItem,
    CListen_Event_MediaListViewItemDeleted,
    CListen_Event_MediaListViewWillDeleteItem,

    CListen_Event_MediaListPlayerPlayed=0x400,
    CListen_Event_MediaListPlayerNextItemSet,
    CListen_Event_MediaListPlayerStopped,

    CListen_Event_MediaDiscovererStarted=0x500,
    CListen_Event_MediaDiscovererEnded,

    CListen_Event_VlmMediaAdded=0x600,
    CListen_Event_VlmMediaRemoved,
    CListen_Event_VlmMediaChanged,
    CListen_Event_VlmMediaInstanceStarted,
    CListen_Event_VlmMediaInstanceStopped,
    CListen_Event_VlmMediaInstanceStatusInit,
    CListen_Event_VlmMediaInstanceStatusOpening,
    CListen_Event_VlmMediaInstanceStatusPlaying,
    CListen_Event_VlmMediaInstanceStatusPause,
    CListen_Event_VlmMediaInstanceStatusEnd,
    CListen_Event_VlmMediaInstanceStatusError,
};

private:
	string m_strName;
};
