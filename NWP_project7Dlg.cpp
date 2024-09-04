
// NWP_project7Dlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "NWP_project7.h"
#include "NWP_project7Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CAboutDlg dialog used for App About (not needed here for now)
class CAboutDlg : public CDialogEx {

public:
};

// CNWPproject7Dlg dialog

CNWPproject7Dlg::CNWPproject7Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_NWP_PROJECT7_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CNWPproject7Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TABCONTROL, m_tabcontrol);
}

BEGIN_MESSAGE_MAP(CNWPproject7Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TCN_SELCHANGE, AFX_ID_PREVIEW_PREV, &CNWPproject7Dlg::OnTcnSelChangeIdPreviewPrev)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TABCONTROL, &CNWPproject7Dlg::OnSelChangeTabControl)
END_MESSAGE_MAP()


// CNWPproject7Dlg message handlers

BOOL CNWPproject7Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the minimize and maximize buttons
	LONG style = GetWindowLong(this->GetSafeHwnd(), GWL_STYLE);
	style |= WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU;
	SetWindowLong(this->GetSafeHwnd(), GWL_STYLE, style);

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	CTabCtrl* pTabCtrl = (CTabCtrl*)GetDlgItem(IDC_TABCONTROL);
	main_tab.Create(IDD_MAINTAB, pTabCtrl);
	list_tab.Create(IDD_LISTTAB, pTabCtrl);
	settings_tab.Create(IDD_SETTINGSTAB, pTabCtrl);
	m_tabcontrol.SetCurSel(0);
	
	TCITEM main_item;
	main_item.mask = TCIF_TEXT | TCIF_PARAM;
	main_item.lParam = (LPARAM)&main_tab;
	main_item.pszText = _T("Specific");
	pTabCtrl->InsertItem(0, &main_item);

	TCITEM list_item;
	list_item.mask = TCIF_TEXT | TCIF_PARAM;
	list_item.lParam = (LPARAM)&list_tab;
	list_item.pszText = _T("BuyerData");
	pTabCtrl->InsertItem(1, &list_item); 

	TCITEM settings_item;
	settings_item.mask = TCIF_TEXT | TCIF_PARAM;
	settings_item.lParam = (LPARAM)&settings_tab;
	settings_item.pszText = _T("AdditionalSettings");
	pTabCtrl->InsertItem(2, &settings_item);


	CRect rc_main_tab;
	pTabCtrl->GetItemRect(0, &rc_main_tab);
	main_tab.SetWindowPos(nullptr, rc_main_tab.left, rc_main_tab.bottom + 1, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

	CRect rc_list_tab;
	pTabCtrl->GetItemRect(0, &rc_list_tab);
	list_tab.SetWindowPos(nullptr, rc_list_tab.left, rc_list_tab.bottom + 1, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

	CRect rc_settings_tab;
	pTabCtrl->GetItemRect(0, &rc_settings_tab);
	settings_tab.SetWindowPos(nullptr, rc_settings_tab.left, rc_settings_tab.bottom + 1, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

	main_tab.ShowWindow(SW_SHOW);
	list_tab.ShowWindow(SW_HIDE);
	settings_tab.ShowWindow(SW_HIDE);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CNWPproject7Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CNWPproject7Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CNWPproject7Dlg::OnSelChangeTabControl(NMHDR* pNMHDR, LRESULT* pResult) {
	*pResult = 0;
	int selected = m_tabcontrol.GetCurSel();

	switch (selected) {
	case 0:
		main_tab.ShowWindow(SW_SHOW);
		list_tab.ShowWindow(SW_HIDE);
		settings_tab.ShowWindow(SW_HIDE);
		break;
	case 1:
		main_tab.ShowWindow(SW_HIDE);
		list_tab.ShowWindow(SW_SHOW);
		settings_tab.ShowWindow(SW_HIDE);
		break;
	case 2:
		main_tab.ShowWindow(SW_HIDE);
		list_tab.ShowWindow(SW_HIDE);
		settings_tab.ShowWindow(SW_SHOW);
		break;
	}
}

void CNWPproject7Dlg::OnTcnSelChangeIdPreviewPrev(NMHDR* pNMHDR, LRESULT* pResult) {
	*pResult = 0;
}