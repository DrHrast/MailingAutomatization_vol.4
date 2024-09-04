// CSettingsTab.cpp : implementation file
//

#include "pch.h"
#include "NWP_project7.h"
#include "afxdialogex.h"
#include "CSettingsTab.h"


// CSettingsTab dialog

IMPLEMENT_DYNAMIC(CSettingsTab, CDialogEx)

CSettingsTab::CSettingsTab(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SETTINGSTAB, pParent)
{

}

CSettingsTab::~CSettingsTab()
{
}

void CSettingsTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSettingsTab, CDialogEx)
END_MESSAGE_MAP()


// CSettingsTab message handlers
