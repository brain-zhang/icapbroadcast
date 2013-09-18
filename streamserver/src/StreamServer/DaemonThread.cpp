// DaemonThread.cpp : 实现文件
//

#include "stdafx.h"
#include "StreamServer.h"
#include "DaemonThread.h"
#include "Util.h"
#include "INISet.h"


// CDaemonThread

IMPLEMENT_DYNCREATE(CDaemonThread, CWinThread)

CDaemonThread::CDaemonThread()
{
}

CDaemonThread::~CDaemonThread()
{
}

BOOL CDaemonThread::InitInstance()
{
	// TODO: 在此执行任意逐线程初始化
	::SetTimer(NULL, NULL, SCAN_TIME_INTERVAL, NULL);
	return TRUE;
}

int CDaemonThread::ExitInstance()
{
	// TODO: 在此执行任意逐线程清理
	return CWinThread::ExitInstance();
}

int CDaemonThread::Run()
{
	MSG   msg;
	while(::GetMessage(&msg,NULL,0,0))
	{
		switch(msg.message)
		{
		case WM_TIMER:
			DaemonDeviceScan();
			break;
		default:
			break;
		}
	}
	return   CWinThread::Run();
} 

void CDaemonThread::DaemonDeviceScan() const
{
	CUtil::ScanAudioDevices();
	CUtil::ScanLedDevices();
}

BEGIN_MESSAGE_MAP(CDaemonThread, CWinThread)
END_MESSAGE_MAP()


// CDaemonThread 消息处理程序
