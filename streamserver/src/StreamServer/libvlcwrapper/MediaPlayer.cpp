#include "windows.h"
#include "tchar.h"
#include "MediaPlayer.h"

CMediaPlayer::CMediaPlayer(const CMedia *media):m_media(media)
{
	m_mediaPlayer = libvlc_media_player_new_from_media(m_media->GetMedia());
}

CMediaPlayer::~CMediaPlayer(void)
{
	libvlc_media_player_release(m_mediaPlayer);
	OutputDebugString(_T("CMediaPlayer Destruct\n"));
}

void CMediaPlayer::Play(void) const
{
	libvlc_media_player_play(m_mediaPlayer);			 
}