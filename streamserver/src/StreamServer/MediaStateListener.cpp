#include "stdafx.h"
#include "MediaStateListener.h"

CMediaStateListener::CMediaStateListener():m_strName("")
{
}

CMediaStateListener::CMediaStateListener(string strName):m_strName(strName)
{
}

CMediaStateListener::~CMediaStateListener(void)
{
}

void CMediaStateListener::Update(libvlc_event_e eEvent, const string &info)
{
	//��ȡ�¼�����
	//����......
}

string CMediaStateListener::GetName()
{
	return m_strName;
}