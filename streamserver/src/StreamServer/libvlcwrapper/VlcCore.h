#pragma once

//���ڰ�װlibvlc���ຯ����������չ
#include <vlc/vlc.h>

class CVlcCore
{
public:
	CVlcCore(const char * const vlc_argc[], int len);
	~CVlcCore(void);

public:
	libvlc_instance_t* GetLibvlcInstance(void) const;

private:
	libvlc_instance_t *m_inst;
};
