#pragma once
#include "afxdialogex.h"
#include <afxcontrolbars.h>
#include <afxdb.h>
#include <vector>
#include <string>

struct SignaturesFromDB
{
	int id;
	CString SignatureName;
	CString SignatureContent;
};

struct SenderEmails {
	int id;
	CString email;
};

// CMainTab dialog

class CMainTab : public CDialogEx
{
	DECLARE_DYNAMIC(CMainTab)

private:
	CDatabase* dbContext;
	CMFCEditBrowseCtrl root_ctrl;
	CMFCEditBrowseCtrl invoice_ctrl;
	CMFCEditBrowseCtrl dnote_ctrl;
	CComboBox email_combo;
	CComboBox signature_combo;
	CComboBox time_combo;
	CString time_setter;
	int signatureId;
	std::vector<SignaturesFromDB> signaturesList;
	std::vector< SenderEmails> senderList;
	void DisableControls();
	void EnableControls();
	void FolderWatcher();

	bool stopWatching = false;

public:
	CString root_directory_path;
	CString invoice_archive_path;
	CString dnote_archive_path;
	CMainTab(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CMainTab();
	void SetDatabase(CDatabase* pDatabase) { dbContext = pDatabase; }
	void SelectSignatureCombo(int id);
	CString GetSignatureId(CString name);
	void GetAllSignatures();
	void GetAllSenderMails();
	CString GetCurrentTime();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAINTAB };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support


protected:
	HICON m_hIcon;
	CBrush m_groupBoxBrush;

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL CMainTab::OnInitDialog();
	void LoadGeneralSettingsFromDb();
	afx_msg void OnBnClickedButtonstart();
	afx_msg void OnBnClickedButtonstop();
	afx_msg void OnEnChangeBrowserootdirectory();
	afx_msg void OnEnChangeBrowseiarchive();
	afx_msg void OnEnChangeBrowsednarchive();
	afx_msg void OnCbnSelchangeCombotime();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnClose();
};
