#include "windows.h"
#include "tchar.h"
#include "Media.h"

CMedia::CMedia(const CVlcCore *core, const char *path):m_core(core)
{
	m_media = libvlc_media_new_path(m_core->GetLibvlcInstance(), path);
}

CMedia::~CMedia(void)
{
	libvlc_media_release(m_media);
	OutputDebugString(_T("CMedia Destruct\n"));
}

libvlc_media_t* CMedia::GetMedia(void) const
{
	return m_media;
}

