#include "windows.h"
#include "tchar.h"
#include "MediaList.h"

CMediaList::CMediaList(CVlcCore *core):m_core(core)
{
	m_medialist = libvlc_media_list_new(m_core->GetLibvlcInstance());
}

CMediaList::~CMediaList(void)
{
	libvlc_media_list_release(m_medialist);
	OutputDebugString(_T("CMediaList Destruct\n"));
}

libvlc_media_list_t* CMediaList::GetMedialist(void) const
{
	return m_medialist;
}

void CMediaList::AddItem(const CMedia *media)
{
	libvlc_media_list_add_media(m_medialist, media->GetMedia());
}

void CMediaList::AddItemByPath(const char *psz_uri)
{
	libvlc_media_list_add_file_content(m_medialist, psz_uri);
}