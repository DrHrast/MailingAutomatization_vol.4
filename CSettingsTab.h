#pragma once
#include "afxdialogex.h"
#include <afxdb.h>
#include <vector>

struct ReceiverEmails {
	int id;
	CString email;
};


// CSettingsTab dialog

class CSettingsTab : public CDialogEx
{
	DECLARE_DYNAMIC(CSettingsTab)

private:
	CDatabase* dbContext;
	CEdit signatureNameCtrl;
	CEdit signatureContentCtrl;
	CButton addSignatureCtrl;
	CEdit senderMailCtrl;
	CEdit receiverMailCtrl;
	CButton addSenderMailCtrl;
	CButton addReceiverMailCtrl;
	CListBox receiverListCtrl;
	std::vector< ReceiverEmails> receiverList;

public:
	CSettingsTab(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CSettingsTab();
	void SetDatabase(CDatabase* pDatabase) { dbContext = pDatabase; }
	void PopulateReceiverList();
	void FIllListDialog();
	BOOL OnInitDialog();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SETTINGSTAB };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonaddsignature();
	afx_msg void OnBnClickedButtonaddsendermail();
	afx_msg void OnBnClickedButtonaddreceivermail();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
};
