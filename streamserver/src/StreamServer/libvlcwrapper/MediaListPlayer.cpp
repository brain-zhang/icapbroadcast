#include "windows.h"
#include "tchar.h"
#include "MediaListPlayer.h"

CMediaListPlayer::CMediaListPlayer(const CVlcCore *core, const CMediaList *medialist):m_core(core), m_medialist(medialist)
{
	m_medialistPlayer = libvlc_media_list_player_new(m_core->GetLibvlcInstance());
	libvlc_media_list_player_set_media_list(m_medialistPlayer, m_medialist->GetMedialist());
	SetPlaybackMode(libvlc_playback_mode_repeat);
}

CMediaListPlayer::~CMediaListPlayer(void)
{
	libvlc_media_list_player_release(m_medialistPlayer);
	OutputDebugString(_T("CMediaListPlayer Destruct\n"));
}

void CMediaListPlayer::Play(void) const
{
	libvlc_media_list_player_play(m_medialistPlayer);

}

void CMediaListPlayer::Pause(void) const
{
	libvlc_media_list_player_pause(m_medialistPlayer);
}

void CMediaListPlayer::SetPlaybackMode(libvlc_playback_mode_t e_mode)
{
	libvlc_media_list_player_set_playback_mode(m_medialistPlayer, e_mode);
}

void CMediaListPlayer::ChangeMediaList(const CMediaList *medialist)
{
	libvlc_media_list_player_set_media_list(m_medialistPlayer, medialist->GetMedialist());
}