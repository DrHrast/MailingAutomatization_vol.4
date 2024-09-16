
// NWP_project7Dlg.h : header file
//

#pragma once
#include "CMainTab.h"
#include "CListTab.h"
#include "CSettingsTab.h"
#include <afxdb.h>

// CNWPproject7Dlg dialog
class CNWPproject7Dlg : public CDialogEx
{
// Construction
public:
	CNWPproject7Dlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_NWP_PROJECT7_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	//afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	CDatabase dbContext;

public:
	CTabCtrl m_tabcontrol;
	CMainTab main_tab;
	CListTab list_tab;
	CSettingsTab settings_tab;
	BOOL ConnectToDatabase();
	void CloseDatabase();
	CDatabase* GetDataBase();

	BOOL TableExists(const CString& tableName);

	afx_msg void OnSelChangeTabControl(NMHDR* pNMHDR, LRESULT* pResult);

private:
};
