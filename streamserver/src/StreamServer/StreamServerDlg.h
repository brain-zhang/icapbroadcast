
// StreamServerDlg.h : ͷ�ļ�
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

// CStreamServerDlg �Ի���
class CStreamServerDlg : public CDialog, public CMediaStateListener
{
// ����
public:
	CStreamServerDlg(CWnd* pParent = NULL);	// ��׼���캯��
	~CStreamServerDlg();

// �Ի�������
	enum { IDD = IDD_STREAMSERVER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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

	//��INI�ļ��ж�ȡ��Ϣ
	void GetIPFromINIFile(CString &strip);
	void GetPortFromINIFile(CString &strport);

	//����CMediaStateListener::Update()
	virtual void Update(libvlc_event_e eEvent, const string &info);

private:
	CVlcMediaStream *m_pmediaStream;
	//�洢����IP�㲥��ַ
	CString m_strIP;
	//�洢�˿�
	CString m_strPort;

	//�Է���˷�����Ϣ,socket�ӿ�
	CSockAudiosw m_audiosw;

	//��LED�����Ϳ�����Ϣ
	CLedCtrl m_led;

	//��̨�̣߳�ÿ��5����ɨ���豸�����Ѳ��ܷ��ʵ��豸��xml�ļ��б��
	CDaemonThread *m_pthread;

	//�����߳�
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
