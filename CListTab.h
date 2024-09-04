#pragma once
#include "afxdialogex.h"


// CListTab dialog

class CListTab : public CDialogEx
{
	DECLARE_DYNAMIC(CListTab)

public:
	CListTab(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CListTab();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LISTTAB };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
