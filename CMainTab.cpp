// CMainTab.cpp : implementation file
//

#include "pch.h"
#include "NWP_project7.h"
#include "afxdialogex.h"
#include "CMainTab.h"


// CMainTab dialog

IMPLEMENT_DYNAMIC(CMainTab, CDialogEx)

CMainTab::CMainTab(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MAINTAB, pParent)
{

}

CMainTab::~CMainTab()
{
}

void CMainTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMainTab, CDialogEx)
END_MESSAGE_MAP()


// CMainTab message handlers
