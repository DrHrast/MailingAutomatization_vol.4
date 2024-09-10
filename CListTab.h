#pragma once
#include "afxdialogex.h"
#include <afxdb.h>


// CListTab dialog

class CListTab : public CDialogEx
{
	DECLARE_DYNAMIC(CListTab)

private:
	CDatabase *dbContext;
	CListCtrl buyerList;
	CEdit addVatCtrl;
	CEdit addPrimaryMailCtrl;
	CEdit addSecondaryMailCtrl;
	CEdit addShortNameCtrl;
	CEdit addNameCtrl;
	CEdit updateVatCtrl;
	CEdit updatePrimaryMailCtrl;
	CEdit updateSecondaryMailCtrl;

public:
	CListTab(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CListTab();
	void SetDatabase(CDatabase* pDatabase) { dbContext = pDatabase; }
	void FillListDialog();
	void AddNewBuyerToDb();
	void UpdateBuyer();
	void ClearFields();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LISTTAB };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL CListTab::OnInitDialog();
	afx_msg void OnBnClickedButtonaddadd();
	afx_msg void OnBnClickedButtonaddclear();
	afx_msg void OnBnClickedButtonupclear();
	afx_msg void OnBnClickedButtonupadd();
};
