#pragma once

#include "vlc/vlc.h"
#include "VlcCore.h"

class CMedia
{
public:
	CMedia(const CVlcCore *core, const char *path);
	~CMedia(void);

public:
	libvlc_media_t* GetMedia(void) const;

private:
	const CVlcCore *m_core;
	libvlc_media_t *m_media;
	
};
