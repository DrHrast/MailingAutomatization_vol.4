#pragma once
#include "afxdialogex.h"


// CSettingsTab dialog

class CSettingsTab : public CDialogEx
{
	DECLARE_DYNAMIC(CSettingsTab)

public:
	CSettingsTab(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CSettingsTab();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SETTINGSTAB };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
