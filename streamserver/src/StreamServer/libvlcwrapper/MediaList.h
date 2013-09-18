#pragma once

#include "vlc/vlc.h"
#include "VlcCore.h"
#include "Media.h"

class CMediaList
{
public:
	CMediaList(CVlcCore* core);
	~CMediaList(void);

public:
	libvlc_media_list_t *GetMedialist(void) const;
	void AddItem(const CMedia *media);
	void AddItemByPath(const char *psz_uri);

private:
	CVlcCore *m_core;
	libvlc_media_list_t *m_medialist;
};
