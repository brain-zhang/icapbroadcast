#pragma once

#include "vlc/vlc.h"
#include "Media.h"

class CMediaPlayer
{
public:
	CMediaPlayer(const CMedia *media);
	~CMediaPlayer(void);

public:
	void Play(void) const;

private:
	const CMedia *m_media;
	libvlc_media_player_t *m_mediaPlayer;
};
