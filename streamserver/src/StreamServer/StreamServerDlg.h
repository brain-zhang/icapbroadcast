
// StreamServerDlg.h : 头文件
//

#pragma once

#include "VlcMediaStream.h"
#include "MediaStateListener.h"
#include "LedCtrl.h"
#include "SockAudiosw.h"
#include "DaemonThread.h"
#include "INISet.h"
#include "afxcmn.h"
#include "afxwin.h"

using namespace std;

// CStreamServerDlg 对话框
class CStreamServerDlg : public CDialog, public CMediaStateListener
{
// 构造
public:
	CStreamServerDlg(CWnd* pParent = NULL);	// 标准构造函数
	~CStreamServerDlg();

// 对话框数据
	enum { IDD = IDD_STREAMSERVER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg BOOL PreTranslateMessage(MSG* pMsg);
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedBtnaddfiles();
	afx_msg void OnBnClickedBtnplay();
	afx_msg void OnBnClickedBtndelfiles();
	afx_msg void OnBnClickedBtnsetip();

	void PlayAndPause(void);

	//从INI文件中读取信息
	void GetIPFromINIFile(CString &strip);
	void GetPortFromINIFile(CString &strport);

	//重载CMediaStateListener::Update()
	virtual void Update(libvlc_event_e eEvent, const string &info);

private:
	CVlcMediaStream *m_pmediaStream;
	//存储本地IP广播地址
	CString m_strIP;
	//存储端口
	CString m_strPort;

	//对服务端发送消息,socket接口
	CSockAudiosw m_audiosw;

	//对LED屏发送控制信息
	CLedCtrl m_led;

	//后台线程，每隔5分钟扫描设备，并把不能访问的设备在xml文件中标出
	CDaemonThread *m_pthread;

	//报警线程
	CWinThread *m_pAudioAlertThread;

	BOOL m_bAudioALertThread;


public:
	CListBox m_ctlList;
	CButton m_btnPlay;
	CEdit m_ctlEdit;
	CIPAddressCtrl m_localIP;
	CEdit m_port;
	afx_msg void OnBnClickedBtnurgent();
	CButton m_btnUrgent;
	afx_msg void OnBnClickedBtnmicro();
	CButton m_btnMicro;
};
