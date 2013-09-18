#pragma once

#include "windef.h"

#include <string>
#include <fstream>
#include <map>

using namespace std;


/*添加屏幕*/
typedef int(__stdcall *AddScreen)(int nControlType,int nScreenNo,int nWidth,int nHeight,int nScreenType,
								  int nPixelMode,int nDataDA,int nDataOE,int nRowOrder,int nFreqPar,
								  char* pCom,int nBaud,char* pSocketIP,int nSocketPort);

/*添加屏幕节目*/
typedef int(__stdcall *AddScreenProgram)(int nScreenNo,int nProgramType,int nPlayLength,int nStartYear,
										 int nStartMonth,int nStartDay,int nEndYear,int nEndMonth,int nEndDay,
										 int nMonPlay,int nTuesPlay,int nWedPlay,int nThursPlay,int nFriPlay,
										 int nSatPlay,int nSunPlay,int nStartHour,int nStartMinute,int nEndHour,
										 int nEndMinute);
/*添加图文区域*/
typedef int(__stdcall *AddScreenProgramBmpTextArea)(int nScreenNo,int nProgramOrd,int nX,int nY,int nWidth,int nHeight);
/*向显示屏指定节目指定区域添加文件*/
typedef int(__stdcall *AddScreenProgramAreaBmpTextFile)(int nScreenNo,int nProgramOrd,int nAreaOrd,char* pFileName,
														char* pFontName,int nFontSize,int nBold,int nFontColor,
														int nStunt,int nRunSpeed,int nShowTime);
/*发送相应命令到显示屏*/
typedef int(__stdcall *SendScreenInfo)(int nScreenNo,int nSendMode,int nSendCmd,int nOtherParam1);

typedef int(__stdcall *InitDLLResource)(HANDLE AHandle);
typedef void(__stdcall *FreeDLLResource)();


class CLedCtrl
{
public:
	CLedCtrl(void);
	~CLedCtrl(void);
	void SetHwnd(HWND wnd);
	void PrepareMessage(const string &ip, const string &content, const string &str_screen_no);
	void CLedCtrl::SendMessage(const string &str_screen_no) const;

private:
	bool IsInited(int screen_no);
	void Inited(int screen_no);

private:
	HINSTANCE hInst;
	AddScreen api_AddScreen;

	AddScreenProgram api_AddScreenProgram;
	AddScreenProgramBmpTextArea api_AddScreenProgramBmpTextArea;
	AddScreenProgramAreaBmpTextFile api_AddScreenProgramAreaBmpTextFile;
	SendScreenInfo api_SendScreenInfo;

	InitDLLResource api_InitDLLResource;
	FreeDLLResource api_FreeDLLResource;

private:
	map<int, bool> m_dictScreens;
};
