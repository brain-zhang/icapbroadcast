
// StreamServer.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������



// CStreamServerApp:
// �йش����ʵ�֣������ StreamServer.cpp
//

class CStreamServerApp : public CWinAppEx
{
public:
	CStreamServerApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CStreamServerApp theApp;