#include "windows.h"
#include "tchar.h"
#include "VlcCore.h"

CVlcCore::CVlcCore(const char * const vlc_argc[], int len)
{
	m_inst = libvlc_new(len, vlc_argc);
}

CVlcCore::~CVlcCore(void)
{
	libvlc_release(m_inst);
	OutputDebugString(_T("CVlcCoreDestruct\n"));
}


libvlc_instance_t* CVlcCore::GetLibvlcInstance(void) const
{
	return m_inst;
}