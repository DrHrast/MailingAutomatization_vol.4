// CMainTab.cpp : implementation file
//

#include <stdio.h>
#include "pch.h"
#include "NWP_project7.h"
#include "afxdialogex.h"
#include "CMainTab.h"
#include <iostream>


// CMainTab dialog

IMPLEMENT_DYNAMIC(CMainTab, CDialogEx)

CMainTab::CMainTab(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MAINTAB, pParent)
{

}

CMainTab::~CMainTab()
{
}

BOOL CMainTab::OnInitDialog() {
	CDialogEx::OnInitDialog();

	m_groupBoxBrush.CreateSolidBrush(RGB(173, 216, 230));

	//TODO: For now manual selection and fill, but later from database
	CMainTab::LoadGeneralSettingsFromDb();

	email_combo.AddString(_T("pehuljek@vsite.hr"));
	email_combo.AddString(_T("soperkov@vsite.hr"));
	email_combo.AddString(_T("petar.huljek@biomax.hr"));
	signature_combo.AddString(_T("Standard"));
	signature_combo.AddString(_T("StandardPh"));
	signature_combo.AddString(_T("StandardSP"));
	signature_combo.AddString(_T("PhBiomStandard"));
	time_combo.AddString(_T("14:00"));
	time_combo.AddString(_T("14:15"));
	time_combo.AddString(_T("14:30"));
	time_combo.AddString(_T("14:45"));
	time_combo.AddString(_T("15:15"));
	time_combo.AddString(_T("15:45"));
	time_combo.AddString(_T("16:00"));
	time_radio1.SetCheck(2);
	signature_combo.SetCurSel(0);
	email_combo.SetCurSel(0);

	/*mainTab_group.SetBackgroundColor(RGB(173, 216, 230));*/

	return TRUE;
}

HBRUSH CMainTab::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) {
	if (nCtlColor == CTLCOLOR_STATIC) {
		if (pWnd->GetDlgCtrlID() == IDC_MAINTABGROUP) {
			pDC->SetBkMode(TRANSPARENT);
			return m_groupBoxBrush;
		}
		if (pWnd->GetDlgCtrlID() == IDC_MAINTABGROUP1) {
			pDC->SetBkMode(TRANSPARENT);
			return m_groupBoxBrush;
		}
		if (pWnd->GetDlgCtrlID() == IDC_MAINTABGROUP2) {
			pDC->SetBkMode(TRANSPARENT);
			return m_groupBoxBrush;
		}
		if (pWnd->GetDlgCtrlID() == IDC_MAINTABGROUP3) {
			pDC->SetBkMode(TRANSPARENT);
			return m_groupBoxBrush;
		}
	}

	return CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CMainTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_BROWSEROOTDIRECTORY, root_ctrl);
	DDX_Control(pDX, IDC_BROWSEIARCHIVE, invoice_ctrl);
	DDX_Control(pDX, IDC_BROWSEDNARCHIVE, dnote_ctrl);
	DDX_Control(pDX, IDC_COMBOBOXSENDERMAIL, email_combo);
	DDX_Control(pDX, IDC_COMBOBOXSIGNATUES, signature_combo);
	DDX_Control(pDX, IDC_RADIO1, time_radio1);
	DDX_Control(pDX, IDC_RADIO2, time_radio2);
	DDX_Control(pDX, IDC_RADIO3, time_radio3);
	DDX_Control(pDX, IDC_COMBOTIME, time_combo);/*
	DDX_Control(pDX, IDC_MAINTABGROUP, mainTab_group);*/
}


BEGIN_MESSAGE_MAP(CMainTab, CDialogEx)
	//ON_EN_CHANGE(IDC_BROWSEROOTDIRECTORY, &CMainTab::OnEnChangeBrowserootdirectory)
	ON_BN_CLICKED(IDC_BUTTONSTART, &CMainTab::OnBnClickedButtonstart)
	ON_BN_CLICKED(IDC_BUTTONSTOP, &CMainTab::OnBnClickedButtonstop)
	ON_EN_CHANGE(IDC_BROWSEROOTDIRECTORY, &CMainTab::OnEnChangeBrowserootdirectory)
	ON_EN_CHANGE(IDC_BROWSEIARCHIVE, &CMainTab::OnEnChangeBrowseiarchive)
	ON_EN_CHANGE(IDC_BROWSEDNARCHIVE, &CMainTab::OnEnChangeBrowsednarchive)
	ON_BN_CLICKED(IDC_RADIO1, &CMainTab::OnBtnClickedRadioTime)
	ON_BN_CLICKED(IDC_RADIO2, &CMainTab::OnBtnClickedRadioTime)
	ON_BN_CLICKED(IDC_RADIO3, &CMainTab::OnBtnClickedRadioTime)
	ON_CBN_SELCHANGE(IDC_COMBOTIME, &CMainTab::OnCbnSelchangeCombotime)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CMainTab message handlers


void CMainTab::OnBnClickedButtonstart()
{
	if (dbContext && dbContext->IsOpen())
	{
		// Fetch data from the controls
		root_ctrl.GetWindowText(root_directory_path);
		invoice_ctrl.GetWindowText(invoice_archive_path);
		dnote_ctrl.GetWindowText(dnote_archive_path);

		CString senderMail, signatureID;
		email_combo.GetWindowText(senderMail);  // Get selected value from email combo
		signature_combo.GetWindowText(signatureID);  // Get selected value from signature 
		OnCbnSelchangeCombotime();

		// Construct the SQL query to insert or update the last row in the GeneralSettings table
		CString sqlQuery;
		sqlQuery.Format(_T("INSERT INTO GeneralSettings (RootPath, InvArchPath, DnArchPath, SenderMail, SignatureId, EndTime) ")
			_T("VALUES ('%s', '%s', '%s', '%s', '%d', '%s')"),
			root_directory_path, invoice_archive_path, dnote_archive_path, senderMail, signatureID, time_setter);

	   /*AfxMessageBox(
			root_directory_path + "\n" + invoice_archive_path +"\n"+  dnote_archive_path
			+"\n"+ combo_box_email + "\n" + combo_box_sig
			+ "\nTime: " + time_setter 
		);*/

		try
		{
			dbContext->ExecuteSQL(sqlQuery);
			AfxMessageBox(_T("Settings saved successfully!"));
		}
		catch (CDBException* e)
		{
			AfxMessageBox(_T("Failed to save settings: ") + e->m_strError);
			e->Delete();
		}
	}
}

void CMainTab::LoadGeneralSettingsFromDb() {
	if (dbContext && dbContext->IsOpen()) {
		CRecordset recordset(dbContext);
		CString sqlQuery = _T("SELECT TOP 1 RootPath, InvArchPath, DnArchPath, SenderMail, SignatureId FROM GeneralSettings ORDER BY ID DESC");
		recordset.Open(CRecordset::forwardOnly, sqlQuery);

		if (!recordset.IsEOF()) {
			CString senderMail;

			recordset.GetFieldValue(_T("RootPath"), root_directory_path);
			recordset.GetFieldValue(_T("InvArchPath"), invoice_archive_path);
			recordset.GetFieldValue(_T("DnArchPath"), dnote_archive_path);
			recordset.GetFieldValue(_T("SenderMail"), senderMail);
			recordset.GetFieldValue(_T("SignatureId"), signatureId);

			root_ctrl.SetWindowText(root_directory_path);
			invoice_ctrl.SetWindowText(invoice_archive_path);
			dnote_ctrl.SetWindowText(dnote_archive_path);
			email_combo.SelectString(-1, senderMail);
			signature_combo.SelectString(-1, CMainTab::GetSignatureName(signatureId));
		}
		recordset.Close();
	}
}

CString CMainTab::GetSignatureName(CString id) {
	CRecordset recordset(dbContext);
	CString sqlQuery = _T("SELECT * FROM Signatures WHERE id = " + id);
	recordset.Open(CRecordset::forwardOnly, sqlQuery);

	if (!recordset.IsEOF()) {
		CString value;
		recordset.GetFieldValue(_T("SignatureName"), value);
		return value;
	}
	recordset.Close();
}

void CMainTab::OnBnClickedButtonstop()
{
	// TODO: Add your control notification handler code here
}

void CMainTab::OnEnChangeBrowserootdirectory()
{
	root_ctrl.GetWindowText(root_directory_path);
}

void CMainTab::OnEnChangeBrowseiarchive()
{
	invoice_ctrl.GetWindowText(invoice_archive_path);
}

void CMainTab::OnEnChangeBrowsednarchive()
{
	dnote_ctrl.GetWindowText(dnote_archive_path);
}

void CMainTab::OnBtnClickedRadioTime()
{
	if (time_radio3.GetCheck() == BST_CHECKED) {
		time_combo.ShowWindow(SW_SHOW);
	}
	else {
		time_combo.ShowWindow(SW_HIDE);
		if (time_radio1.GetCheck() == BST_CHECKED) {
			time_radio1.GetWindowText(time_setter);
		}
		else {
			time_radio2.GetWindowText(time_setter);
		}
	}
}

void CMainTab::OnCbnSelchangeCombotime()
{
	time_combo.GetWindowText(time_setter);
}
