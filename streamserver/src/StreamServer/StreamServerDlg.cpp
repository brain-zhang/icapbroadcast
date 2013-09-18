
// StreamServerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include <atlconv.h>

#include <algorithm>

#include "StreamServer.h"
#include "StreamServerDlg.h"
#include "VlcMediaStream.h"
#include "Util.h"
#include "SSExcept.h"
#include "INISet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CStreamServerDlg 对话框




CStreamServerDlg::CStreamServerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CStreamServerDlg::IDD, pParent), CMediaStateListener("StreamServer"),
		m_strIP(DEFAULT_BROADCAST_ADDRESS), m_strPort(DEFAULT_BROADCAST_PORT), m_pmediaStream(NULL), m_bAudioALertThread(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	//从配置文件中获取ip地址及端口
	GetIPFromINIFile(m_strIP);
	GetPortFromINIFile(m_strPort);

	//开启一个vlc实例
	m_pmediaStream = new CVlcMediaStream();
	m_pmediaStream->SetVlcCore((CUtil::EncodeUtf8((LPCWSTR)m_strIP)).c_str(), (CUtil::EncodeUtf8((LPCWSTR)(m_strPort))).c_str());

	//设置一个监听者，此处只监听"下一首歌曲"这一事件
	m_pmediaStream->AddListener(this);

	//创建后台线程，循环扫描设备
	m_pthread = new CDaemonThread();
	m_pthread->CreateThread();
}

CStreamServerDlg::~CStreamServerDlg()
{
	delete m_pthread;
	delete m_pmediaStream;

	m_pthread = NULL;
	m_pmediaStream = NULL;
}

void CStreamServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ListFiles, m_ctlList);
	DDX_Control(pDX, ID_BtnPlay, m_btnPlay);
	DDX_Control(pDX, IDC_EDIT, m_ctlEdit);
	DDX_Control(pDX, IDC_IPADDRESS, m_localIP);
	DDX_Control(pDX, IDC_EDIT1, m_port);
	DDX_Control(pDX, IDC_BtnUrgent, m_btnUrgent);
	DDX_Control(pDX, IDC_BtnMicro, m_btnMicro);
}

BEGIN_MESSAGE_MAP(CStreamServerDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BtnAddFiles, &CStreamServerDlg::OnBnClickedBtnaddfiles)
	ON_BN_CLICKED(ID_BtnPlay, &CStreamServerDlg::OnBnClickedBtnplay)
	ON_BN_CLICKED(IDC_BtnDelFiles, &CStreamServerDlg::OnBnClickedBtndelfiles)
	ON_BN_CLICKED(IDC_BtnSetIP, &CStreamServerDlg::OnBnClickedBtnsetip)
	ON_BN_CLICKED(IDC_BtnUrgent, &CStreamServerDlg::OnBnClickedBtnurgent)
	ON_BN_CLICKED(IDC_BtnMicro, &CStreamServerDlg::OnBnClickedBtnmicro)
END_MESSAGE_MAP()


// CStreamServerDlg 消息处理程序
BOOL CStreamServerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_ctlEdit.SetWindowText(_T(PROGRAM_RUN_FIRST_INFO_TEXT));
	m_localIP.SetWindowText(m_strIP);
	m_port.SetWindowText(m_strPort);
	m_led.SetHwnd(GetSafeHwnd());

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CStreamServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CStreamServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CStreamServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CStreamServerDlg::OnBnClickedBtnaddfiles()
{
	vector<string> vstr; //保存歌曲路径名称
	CString szFilters = _T("Audio Files (*.mp3)|*.mp3|All Files (*.*)|*.*||");   //定义文件过滤器

	//创建打开文件对话框对象，默认的文件扩展名为 ".doc".
	CFileDialog fileDlg (TRUE, _T("mp3"), _T("*.mp3"),OFN_FILEMUSTEXIST| OFN_ALLOWMULTISELECT | OFN_EXPLORER | OFN_NOCHANGEDIR, szFilters, this);
	TCHAR  szLargeBuf[4096];
	memset(szLargeBuf,'\0',4096);
	fileDlg.m_ofn.lpstrFile = szLargeBuf; 
#ifdef UNICODE
	fileDlg.m_ofn.nMaxFile = 4096;
#else
	fileDlg.m_ofn.nMaxFile = sizeof (szLargeBuf);
#endif

	//调用DoModal()函数显示打开文件对话框
	if(fileDlg.DoModal() == IDOK)
	{
		//m_ctlList.ResetContent();//清空列表框，

		POSITION pos;
		pos = fileDlg.GetStartPosition();//开始遍历用户选择文件列表
		while (pos!=NULL)
		{
			CString filename = fileDlg.GetNextPathName(pos);
			m_ctlList.AddString(filename);//将文件名添加到列表框

			//将歌曲路径暂时保存添加到列表
			vstr.push_back(string(CUtil::EncodeUtf8((LPCWSTR)filename).c_str()));
			
		}
	}

	//将歌曲添加到播放列表中
	m_pmediaStream->AddItemsByPaths(vstr);
}

void CStreamServerDlg::OnBnClickedBtnplay()
{

	//检测列表中是否有歌曲
	if (m_ctlList.GetCount() == 0)
	{
		MessageBox(_T(PLAY_LIST_NULL_ERR), _T(WARNING_INFO), MB_OK);
		return;
	}
	PlayAndPause();
}


void CStreamServerDlg::PlayAndPause()
{
	//处理一下按钮的显示，以及暂停动作
	static BOOL is_playing = FALSE;
	is_playing = !is_playing;

	if (is_playing)
	{
		m_btnPlay.SetWindowText(_T(PAUSE_INFO_TEXT));
		m_pmediaStream->PlayMediaStream();
	}
	else
	{
		m_btnPlay.SetWindowText(_T(PLAY_INFO_TEXT));
		m_pmediaStream->PauseMediaPlay();
	}
}

void CStreamServerDlg::OnBnClickedBtndelfiles()
{
	//存储要删除的歌曲序列号
	vector<int> vno;

	// 删除所选的项
	int nCount = m_ctlList.GetSelCount();
	int* buffer = new int[nCount];
	memset(buffer, -1, sizeof(int)*nCount);

	m_ctlList.GetSelItems(nCount, buffer);

	for(int i = nCount - 1; i >= 0; i--)
	{
		m_ctlList.DeleteString(buffer[i]);
		vno.push_back(buffer[i]);
	}

	m_pmediaStream->DelItemsByIndexes(vno);

	delete[] buffer;
}


// Ctrl+A选择所有的Item,也可以注册系统Hotkey，用于list控件
BOOL CStreamServerDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(WM_KEYDOWN == pMsg->message && _T('A') == pMsg->wParam)
	{
		if(HIBYTE(GetKeyState(VK_CONTROL)))
		{
			m_ctlList.SelItemRange(TRUE, 0, m_ctlList.GetCount());
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CStreamServerDlg::OnBnClickedBtnsetip()
{
	// TODO: 在此添加控件通知处理程序代码
	m_localIP.GetWindowText(m_strIP);
	m_port.GetWindowText(m_strPort);

	WritePrivateProfileString(_T("net"), _T("broadcastip"), m_strIP, _T(INIFILENAME));
	WritePrivateProfileString(_T("net"), _T("port"), m_strPort, _T(INIFILENAME));

	MessageBox(_T(RESET_UPDATE_INFO_TEXT), _T(WARNING_INFO), MB_OK);

	CString oldContent;
	m_ctlEdit.GetWindowText(oldContent);
	m_ctlEdit.SetWindowText(oldContent + CString("\r\n重新设定广播地址\r\n"));
}

void CStreamServerDlg::GetIPFromINIFile(CString &strip)
{
	if(GetPrivateProfileInt(_T("run"), _T("firstrun"), 1, _T(INIFILENAME)))
	{
		//获取本机IP，填入IPAddress中
		WSADATA wsData;  
		::WSAStartup(MAKEWORD(2,2), &wsData);  

		char    HostName[100];
		gethostname(HostName, sizeof(HostName));// 获得本机主机名.

		hostent *hostEntry = gethostbyname(HostName); //根据本机主机名得到本机ip
		CString strIPAddr;
		strIPAddr.Format(_T("%d.%d.%d.%d") ,
			(hostEntry->h_addr_list[0][0]&0x00ff) ,
			(hostEntry->h_addr_list[0][1]&0x00ff) ,
			(hostEntry->h_addr_list[0][2]&0x00ff) ,
			255
			);

		WritePrivateProfileString(_T("net"), _T("port"), strIPAddr, _T(INIFILENAME));
		WritePrivateProfileString(_T("run"), _T("firstrun"), _T("0"), _T(INIFILENAME));
	}

	GetPrivateProfileString(_T("net"), _T("broadcastip"),_T("192.168.1.255"), strip.GetBuffer(30), 30, _T(INIFILENAME));
	strip.ReleaseBuffer();
}

void CStreamServerDlg::GetPortFromINIFile(CString &strport)
{
	CString port;
	GetPrivateProfileString(_T("net"), _T("port"), _T("1234"), strport.GetBuffer(10), 10, _T(INIFILENAME));
	strport.ReleaseBuffer();
}
void CStreamServerDlg::OnBnClickedBtnurgent()
{
	// TODO: 在此添加控件通知处理程序代码
	static BOOL is_enable = FALSE;
	is_enable = !is_enable;

	if(is_enable)
	{
		//开辟两个线程向led控制板和audio控制板发送消息
		//此时不必在意delete，这个版本的MFC AfxBeginThread返回的CWinThread的对象指针所指的对象在默认情况下会自动删除
		AfxBeginThread(CUtil::Alert, &m_led);
		m_pAudioAlertThread = AfxBeginThread(CUtil::AlertAudio, &m_audiosw);

		m_pmediaStream->PlayAlert(string("alarm.mp3"));

		m_bAudioALertThread = TRUE;

		m_btnUrgent.SetWindowText(_T(ALERT_INFO_TEXT));
		m_btnPlay.EnableWindow(FALSE);
	}
	else
	{
		AfxBeginThread(CUtil::CancelAlert, &m_led);
		m_pAudioAlertThread = AfxBeginThread(CUtil::CancelAlertAudio, &m_audiosw);

		m_pmediaStream->PlaySafe();

		m_bAudioALertThread = FALSE;

		m_btnUrgent.SetWindowText(_T(CANCEL_ALERT_INFO_TEXT));
		m_btnPlay.EnableWindow(TRUE);
	}
	PlayAndPause();
}


//"下一首歌"事件发生，重载CMediaStateListener的Update()
void CStreamServerDlg::Update(libvlc_event_e eEvent, const string &info)
{
	string strFileName = CUtil::URLDecode(info);

	//去掉前缀
	strFileName.replace(0, 8, "");

	//显示在下方
	m_ctlEdit.SetWindowText((LPCWSTR)(CUtil::DecodeUtf8(strFileName).c_str()));
}

void CStreamServerDlg::OnBnClickedBtnmicro()
{
	// TODO: 在此添加控件通知处理程序代码
	static bool is_called = false;
	try
	{
		if (!is_called)
		{
			if(m_bAudioALertThread)
			{
				//如果与紧急报警冲突
				m_pmediaStream->PlayAlertPause();
				PlayAndPause();
			}
			m_pmediaStream->PlayMicro();
			m_btnMicro.SetWindowText(_T("停止语音"));
			m_btnPlay.EnableWindow(FALSE);
		}
		else
		{
			if(m_bAudioALertThread)
			{
				//如果与紧急报警冲突
				m_pmediaStream->PlayAlertResume();
				PlayAndPause();
			}
			m_pmediaStream->StopMicro();
			m_btnMicro.SetWindowText(_T("语音广播"));
			m_btnPlay.EnableWindow(TRUE);
		}
	}
	catch(CMediaExcept &e)
	{
		CString s(e.what());
		MessageBox(s,_T(WARNING_INFO));
		return;
	}
	is_called = !is_called;
}
