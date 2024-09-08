#pragma once
#include "afxdialogex.h"
#include <afxdb.h>


// CListTab dialog

class CListTab : public CDialogEx
{
	DECLARE_DYNAMIC(CListTab)

private:
	CDatabase *dbContext;

public:
	CListTab(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CListTab();
	void SetDatabase(CDatabase* pDatabase) { dbContext = pDatabase; }

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LISTTAB };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
