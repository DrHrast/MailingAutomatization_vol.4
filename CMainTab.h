#pragma once
#include "afxdialogex.h"


// CMainTab dialog

class CMainTab : public CDialogEx
{
	DECLARE_DYNAMIC(CMainTab)

public:
	CMainTab(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CMainTab();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAINTAB };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
