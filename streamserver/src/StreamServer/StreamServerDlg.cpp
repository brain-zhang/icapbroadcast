
// StreamServerDlg.cpp : ʵ���ļ�
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


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CStreamServerDlg �Ի���




CStreamServerDlg::CStreamServerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CStreamServerDlg::IDD, pParent), CMediaStateListener("StreamServer"),
		m_strIP(DEFAULT_BROADCAST_ADDRESS), m_strPort(DEFAULT_BROADCAST_PORT), m_pmediaStream(NULL), m_bAudioALertThread(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	//�������ļ��л�ȡip��ַ���˿�
	GetIPFromINIFile(m_strIP);
	GetPortFromINIFile(m_strPort);

	//����һ��vlcʵ��
	m_pmediaStream = new CVlcMediaStream();
	m_pmediaStream->SetVlcCore((CUtil::EncodeUtf8((LPCWSTR)m_strIP)).c_str(), (CUtil::EncodeUtf8((LPCWSTR)(m_strPort))).c_str());

	//����һ�������ߣ��˴�ֻ����"��һ�׸���"��һ�¼�
	m_pmediaStream->AddListener(this);

	//������̨�̣߳�ѭ��ɨ���豸
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


// CStreamServerDlg ��Ϣ�������
BOOL CStreamServerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	m_ctlEdit.SetWindowText(_T(PROGRAM_RUN_FIRST_INFO_TEXT));
	m_localIP.SetWindowText(m_strIP);
	m_port.SetWindowText(m_strPort);
	m_led.SetHwnd(GetSafeHwnd());

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CStreamServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CStreamServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CStreamServerDlg::OnBnClickedBtnaddfiles()
{
	vector<string> vstr; //�������·������
	CString szFilters = _T("Audio Files (*.mp3)|*.mp3|All Files (*.*)|*.*||");   //�����ļ�������

	//�������ļ��Ի������Ĭ�ϵ��ļ���չ��Ϊ ".doc".
	CFileDialog fileDlg (TRUE, _T("mp3"), _T("*.mp3"),OFN_FILEMUSTEXIST| OFN_ALLOWMULTISELECT | OFN_EXPLORER | OFN_NOCHANGEDIR, szFilters, this);
	TCHAR  szLargeBuf[4096];
	memset(szLargeBuf,'\0',4096);
	fileDlg.m_ofn.lpstrFile = szLargeBuf; 
#ifdef UNICODE
	fileDlg.m_ofn.nMaxFile = 4096;
#else
	fileDlg.m_ofn.nMaxFile = sizeof (szLargeBuf);
#endif

	//����DoModal()������ʾ���ļ��Ի���
	if(fileDlg.DoModal() == IDOK)
	{
		//m_ctlList.ResetContent();//����б��

		POSITION pos;
		pos = fileDlg.GetStartPosition();//��ʼ�����û�ѡ���ļ��б�
		while (pos!=NULL)
		{
			CString filename = fileDlg.GetNextPathName(pos);
			m_ctlList.AddString(filename);//���ļ�����ӵ��б��

			//������·����ʱ������ӵ��б�
			vstr.push_back(string(CUtil::EncodeUtf8((LPCWSTR)filename).c_str()));
			
		}
	}

	//��������ӵ������б���
	m_pmediaStream->AddItemsByPaths(vstr);
}

void CStreamServerDlg::OnBnClickedBtnplay()
{

	//����б����Ƿ��и���
	if (m_ctlList.GetCount() == 0)
	{
		MessageBox(_T(PLAY_LIST_NULL_ERR), _T(WARNING_INFO), MB_OK);
		return;
	}
	PlayAndPause();
}


void CStreamServerDlg::PlayAndPause()
{
	//����һ�°�ť����ʾ���Լ���ͣ����
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
	//�洢Ҫɾ���ĸ������к�
	vector<int> vno;

	// ɾ����ѡ����
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


// Ctrl+Aѡ�����е�Item,Ҳ����ע��ϵͳHotkey������list�ؼ�
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_localIP.GetWindowText(m_strIP);
	m_port.GetWindowText(m_strPort);

	WritePrivateProfileString(_T("net"), _T("broadcastip"), m_strIP, _T(INIFILENAME));
	WritePrivateProfileString(_T("net"), _T("port"), m_strPort, _T(INIFILENAME));

	MessageBox(_T(RESET_UPDATE_INFO_TEXT), _T(WARNING_INFO), MB_OK);

	CString oldContent;
	m_ctlEdit.GetWindowText(oldContent);
	m_ctlEdit.SetWindowText(oldContent + CString("\r\n�����趨�㲥��ַ\r\n"));
}

void CStreamServerDlg::GetIPFromINIFile(CString &strip)
{
	if(GetPrivateProfileInt(_T("run"), _T("firstrun"), 1, _T(INIFILENAME)))
	{
		//��ȡ����IP������IPAddress��
		WSADATA wsData;  
		::WSAStartup(MAKEWORD(2,2), &wsData);  

		char    HostName[100];
		gethostname(HostName, sizeof(HostName));// ��ñ���������.

		hostent *hostEntry = gethostbyname(HostName); //���ݱ����������õ�����ip
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	static BOOL is_enable = FALSE;
	is_enable = !is_enable;

	if(is_enable)
	{
		//���������߳���led���ư��audio���ư巢����Ϣ
		//��ʱ��������delete������汾��MFC AfxBeginThread���ص�CWinThread�Ķ���ָ����ָ�Ķ�����Ĭ������»��Զ�ɾ��
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


//"��һ�׸�"�¼�����������CMediaStateListener��Update()
void CStreamServerDlg::Update(libvlc_event_e eEvent, const string &info)
{
	string strFileName = CUtil::URLDecode(info);

	//ȥ��ǰ׺
	strFileName.replace(0, 8, "");

	//��ʾ���·�
	m_ctlEdit.SetWindowText((LPCWSTR)(CUtil::DecodeUtf8(strFileName).c_str()));
}

void CStreamServerDlg::OnBnClickedBtnmicro()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	static bool is_called = false;
	try
	{
		if (!is_called)
		{
			if(m_bAudioALertThread)
			{
				//��������������ͻ
				m_pmediaStream->PlayAlertPause();
				PlayAndPause();
			}
			m_pmediaStream->PlayMicro();
			m_btnMicro.SetWindowText(_T("ֹͣ����"));
			m_btnPlay.EnableWindow(FALSE);
		}
		else
		{
			if(m_bAudioALertThread)
			{
				//��������������ͻ
				m_pmediaStream->PlayAlertResume();
				PlayAndPause();
			}
			m_pmediaStream->StopMicro();
			m_btnMicro.SetWindowText(_T("�����㲥"));
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
