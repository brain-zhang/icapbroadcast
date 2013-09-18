#include "stdafx.h"
#include <windows.h>
#include <sstream>

#include "LedCtrl.h"
#include "SSExcept.h"
#include "INISet.h"

//------------------------------------------------------------------------------
// ����������
#define CONTROLLER_TYPE_3T   0x10
#define CONTROLLER_TYPE_3A   0x20
#define CONTROLLER_TYPE_3A1   0x21
#define CONTROLLER_TYPE_3A2   0x22
#define CONTROLLER_TYPE_3M   0x30

#define CONTROLLER_TYPE_4A1   0x0141
#define CONTROLLER_TYPE_4A2   0x0241
#define CONTROLLER_TYPE_4A3   0x0341
#define CONTROLLER_TYPE_4AQ   0x1041
#define CONTROLLER_TYPE_4A   0x0041

#define CONTROLLER_TYPE_4M0   0x0242
#define CONTROLLER_TYPE_4M1   0x0142
#define CONTROLLER_TYPE_4M   0x0042
#define CONTROLLER_TYPE_4MC   0x0C42
#define CONTROLLER_TYPE_4E   0x0044
#define CONTROLLER_TYPE_4C   0x0043
#define CONTROLLER_TYPE_4E1   0x0144

//------------------------------------------------------------------------------
//                                                                              
// ������ͨѶģʽ
#define SEND_MODE_COMM   0
#define SEND_MODE_NET   2
//                                                                              

//                                                                              
// �û�������Ϣ�����
#define SEND_CMD_PARAMETER   0xA1FF //������������
#define SEND_CMD_SCREENSCAN   0xA1FE //����ɨ�跽ʽ��
#define SEND_CMD_SENDALLPROGRAM   0xA1F0 //�������н�Ŀ��Ϣ��
#define SEND_CMD_POWERON   0xA2FF //ǿ�ƿ���
#define SEND_CMD_POWEROFF   0xA2FE //ǿ�ƹػ�
#define SEND_CMD_TIMERPOWERONOFF   0xA2FD //��ʱ���ػ�
#define  SEND_CMD_CANCEL_TIMERPOWERONOFF   0xA2FC //ȡ����ʱ���ػ�
#define SEND_CMD_RESIVETIME   0xA2FB //У��ʱ�䡣
#define SEND_CMD_ADJUSTLIGHT   0xA2FA //���ȵ�����
                                                                     

#define SCREEN_NO   1
#define SCREEN_WIDTH   64
#define SCREEN_HEIGHT   16
#define SCREEN_TYPE   1
#define SCREEN_DATADA   0
#define SCREEN_DATAOE   1
#define SCREEN_COMM   "COM1"
#define SCREEN_BAUD   57600
#define SCREEN_ROWORDER   0
#define SCREEN_FREQPAR   0
#define SCREEN_SOCKETIP   "192.168.1.3"
#define SCREEN_SOCKETPORT   5005

#define CHECK_THROW(regx, what) if(!regx) {throw CLedExcept(what); return;}


CLedCtrl::CLedCtrl(void)
{
	hInst = NULL;
	hInst = LoadLibrary(_T("BX_IV.dll"));

	api_AddScreen=(AddScreen)GetProcAddress(hInst,"AddScreen");
	api_AddScreenProgram=(AddScreenProgram)GetProcAddress(hInst,"AddScreenProgram");
	api_AddScreenProgramBmpTextArea=(AddScreenProgramBmpTextArea)GetProcAddress(hInst,"AddScreenProgramBmpTextArea");
	api_AddScreenProgramAreaBmpTextFile=(AddScreenProgramAreaBmpTextFile)GetProcAddress(hInst,"AddScreenProgramAreaBmpTextFile");
	api_SendScreenInfo=(SendScreenInfo)GetProcAddress(hInst,"SendScreenInfo");

	api_InitDLLResource=(InitDLLResource)GetProcAddress(hInst,"InitDLLResource");
	api_FreeDLLResource=(FreeDLLResource)GetProcAddress(hInst,"FreeDLLResource");	
}

CLedCtrl::~CLedCtrl(void)
{
	api_FreeDLLResource();
	FreeLibrary(hInst);
}


void CLedCtrl::SetHwnd(HWND wnd)
{
	api_InitDLLResource(wnd);
}

void CLedCtrl::PrepareMessage(const string &ip, const string &content, const string &str_screen_no)
{
	int nResult;

	istringstream is(str_screen_no);
	int iscreen_no = 0;
	is>>iscreen_no;

	if(!IsInited(iscreen_no))
	{
		//�����Ļ
		nResult = api_AddScreen(CONTROLLER_TYPE_4M0, iscreen_no, SCREEN_WIDTH
			, SCREEN_HEIGHT, SCREEN_TYPE, 1,SCREEN_DATADA, SCREEN_DATAOE
			, SCREEN_ROWORDER, SCREEN_FREQPAR,SCREEN_COMM
			, SCREEN_BAUD,const_cast<char  *>(ip.c_str()), SCREEN_SOCKETPORT);
		CHECK_THROW(nResult == 0, string(LED_APPEND_SCREEN_ERR) + ip);

		//��������
		nResult = api_SendScreenInfo(iscreen_no, SEND_MODE_NET, SEND_CMD_PARAMETER, 0);
		CHECK_THROW(nResult == 0, string(LED_SET_SCREEN_ATTR_ERR) + ip);

		//��ӽ�Ŀ
		nResult = api_AddScreenProgram(iscreen_no, 0, 0, 65535, 12, 3, 2011, 11, 26, 1, 1, 1, 1, 1, 1, 1, 8, 0, 18, 0);
		CHECK_THROW(nResult == 0, string(LED_APPEND_PROGRAM_ERR) + ip);

		//�������
		nResult = api_AddScreenProgramBmpTextArea(iscreen_no, 0, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
		CHECK_THROW(nResult == 0, string(LED_APPEND_AREA_ERR) + ip);

		Inited(iscreen_no);
	}


	//�����������Ϣ
	char filename[] = TEMP_LED_SCREEN_TXT_NAME;
	char fonttype[] = LED_SCREENS_FONT;
	ofstream outfile(filename);
	outfile<<content;
	nResult = api_AddScreenProgramAreaBmpTextFile(SCREEN_NO, 0, 0, TEMP_LED_SCREEN_TXT_NAME, LED_SCREENS_FONT, 12, 0, 1, 4, 3, 0);
	CHECK_THROW(nResult == 0, string(LED_APPEND_INFO_TO_AREA_ERR) + ip);
}

void CLedCtrl::SendMessage(const string &str_screen_no) const
{
	//����
	istringstream is(str_screen_no);
	int iscreen_no = 0;
	is>>iscreen_no;
	int nResult = api_SendScreenInfo(iscreen_no, SEND_MODE_NET, SEND_CMD_SENDALLPROGRAM, 0);
	CHECK_THROW(nResult == 0, string(CLICK_ALERT_BTN_TOO_MANY));
}


//�����豸�Ƿ��Ѿ�����ʼ����
bool CLedCtrl::IsInited(int screen_no)
{
	return m_dictScreens[screen_no];
}

//���豸�Ѿ�����ʼ����
void CLedCtrl::Inited(int screen_no)
{
	m_dictScreens[screen_no] = true;
}