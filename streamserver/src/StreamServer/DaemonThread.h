#pragma once

#include "SockAudiosw.h"

// CDaemonThread
//�����ں�̨����ʱɨ�������豸�Ƿ���Է���
//���ܷ��ʵ��豸��xml�ļ��������

class CDaemonThread : public CWinThread
{
	DECLARE_DYNCREATE(CDaemonThread)

public:	
	CDaemonThread();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
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


