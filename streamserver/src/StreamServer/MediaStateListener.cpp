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
	//获取事件类型
	//处理......
}

string CMediaStateListener::GetName()
{
	return m_strName;
}