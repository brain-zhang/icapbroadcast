#pragma once

#include <memory>
#include "vlc/vlc.h"
#include "VlcCore.h"
#include "MediaList.h"

class CMediaListPlayer
{
public:
	CMediaListPlayer(const CVlcCore *core, const CMediaList *medialist);
	~CMediaListPlayer(void);

public:
	void Play(void) const;
	void Pause(void) const;
	void SetPlaybackMode(libvlc_playback_mode_t e_mode = libvlc_playback_mode_loop);
	void ChangeMediaList(const CMediaList *medialist);

private:
	const CVlcCore *m_core;
	const CMediaList *m_medialist;
	libvlc_media_list_player_t *m_medialistPlayer;
};
