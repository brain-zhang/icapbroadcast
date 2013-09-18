#pragma once

#include "SockAudiosw.h"

// CDaemonThread
//用于在后台，定时扫描所有设备是否可以访问
//不能访问的设备在xml文件中做标记

class CDaemonThread : public CWinThread
{
	DECLARE_DYNCREATE(CDaemonThread)

public:	
	CDaemonThread();           // 动态创建所使用的受保护的构造函数
	virtual ~CDaemonThread();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual int CDaemonThread::Run();

private:
	void DaemonDeviceScan(void) const;

private:
	CSockAudiosw m_sockAudio;

protected:
	DECLARE_MESSAGE_MAP()
};


