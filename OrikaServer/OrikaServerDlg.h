

#pragma once
#include "resource.h"

// COrikaServerDlg dialog
class COrikaServerDlg : public CDialogEx
{
// Construction
public:
	
	COrikaServerDlg(CWnd* pParent = NULL);	// standard constructor
	void data_read_from_MSMQ();
	void checkandremovetoken();
// Dialog Data
	enum { IDD = IDD_ORIKASERVER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	CButton m_btnstart;
	CButton m_btnstop;
	CButton m_btnexit;
	CEdit   m_txtport;

	

	

	

	

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedStart();
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnClose();
	
	
	afx_msg void OnBnClickedExit();
	afx_msg void OnBnClickedStop();





	

	afx_msg void OnBnClickedImportMasterdata();
};
