#pragma once
#include "afxdialogex.h"
#include <afxdb.h>


// CSettingsTab dialog

class CSettingsTab : public CDialogEx
{
	DECLARE_DYNAMIC(CSettingsTab)

private:
	CDatabase* dbContext;

public:
	CSettingsTab(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CSettingsTab();
	void SetDatabase(CDatabase* pDatabase) { dbContext = pDatabase; }

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SETTINGSTAB };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
