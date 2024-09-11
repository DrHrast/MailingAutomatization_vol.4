// CMainTab.cpp : implementation file
//

#include <stdio.h>
#include "pch.h"
#include "NWP_project7.h"
#include "afxdialogex.h"
#include "afxstr.h"
#include "CMainTab.h"
#include <iostream>
#include <Python.h>
#include <Windows.h>
#include <ctime>

UINT CMainTab::WatcherThreadProc(LPVOID pParam)
{
	CMainTab* pThis = (CMainTab*)pParam;

	// Get the directory path
	std::wstring directory(pThis->root_directory_path.GetString());

	HANDLE hDir = CreateFileW(
		directory.c_str(),
		FILE_LIST_DIRECTORY,
		FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
		NULL,
		OPEN_EXISTING,
		FILE_FLAG_BACKUP_SEMANTICS,
		NULL
	);

	if (hDir == INVALID_HANDLE_VALUE)
	{
		AfxMessageBox(_T("Failed to open directory handle."));
		return 1;
	}

	char buffer[1024];
	DWORD bytesReturned;

	while (!pThis->stopWatching)
	{
		// Check if the stop event has been signaled
		if (WaitForSingleObject(pThis->m_hStopEvent, 1000) == WAIT_OBJECT_0)
		{
			break;
		}

		// Check the time
		CString currentTime = pThis->GetCurrentTime();
		if (currentTime.Compare(pThis->time_setter) == 0)
		{
			AfxMessageBox(_T("Time reached!"));
			break;
		}

		if (ReadDirectoryChangesW(
			hDir,
			&buffer,
			sizeof(buffer),
			FALSE, // Subdirectories
			FILE_NOTIFY_CHANGE_FILE_NAME,
			&bytesReturned,
			NULL,
			NULL
		))
		{
			FILE_NOTIFY_INFORMATION* pNotify;
			int offset = 0;

			do {
				pNotify = (FILE_NOTIFY_INFORMATION*)&buffer[offset];
				std::wstring fileName(pNotify->FileName, pNotify->FileNameLength / sizeof(WCHAR));

				if (pNotify->Action == FILE_ACTION_ADDED)
				{
					CString message;
					message.Format(_T("New file detected: %s"), fileName.c_str());
					AfxMessageBox(message);
				}

				offset += pNotify->NextEntryOffset;
			} while (pNotify->NextEntryOffset != 0);
		}
		else
		{
			AfxMessageBox(_T("Error reading directory changes."));
		}
	}

	CloseHandle(hDir);

	return 0;
}


// CMainTab dialog

IMPLEMENT_DYNAMIC(CMainTab, CDialogEx)

void CMainTab::DisableControls()
{
	root_ctrl.EnableWindow(false);
	invoice_ctrl.EnableWindow(false);
	dnote_ctrl.EnableWindow(false);
	email_combo.EnableWindow(false);
	signature_combo.EnableWindow(false);
	time_combo.EnableWindow(false);
}

void CMainTab::EnableControls()
{
	root_ctrl.EnableWindow(true);
	invoice_ctrl.EnableWindow(true);
	dnote_ctrl.EnableWindow(true);
	email_combo.EnableWindow(true);
	signature_combo.EnableWindow(true);
	time_combo.EnableWindow(true);
}

CMainTab::CMainTab(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MAINTAB, pParent)
{

}

CMainTab::~CMainTab()
{
	if (m_pWatcherThread)
	{
		OnBnClickedButtonstop(); 
	}

	if (m_hStopEvent)
	{
		CloseHandle(m_hStopEvent);
		m_hStopEvent = NULL;
	}
}

BOOL CMainTab::OnInitDialog() {
	CDialogEx::OnInitDialog();

	m_groupBoxBrush.CreateSolidBrush(RGB(173, 216, 230));
	stopWatching = FALSE;

	//DID_IT: For now manual selection and fill, but later from database
	GetAllSignatures();
	for (int i = 0; i < signaturesList.size(); ++i) {
		signature_combo.InsertString(i, (signaturesList[i].SignatureName));
	}

	GetAllSenderMails();
	for (int i = 0; i < senderList.size(); ++i) {
		email_combo.InsertString(i, (senderList[i].email));
	}
	LoadGeneralSettingsFromDb();

	//From db load
	/*email_combo.AddString(_T("pehuljek@vsite.hr"));
	email_combo.AddString(_T("soperkov@vsite.hr"));
	email_combo.AddString(_T("petar.huljek@biomax.hr"));*/

	//From db load
	/*signature_combo.AddString(_T("Standard"));
	signature_combo.AddString(_T("StandardPh"));
	signature_combo.AddString(_T("StandardSP"));
	signature_combo.AddString(_T("PhBiomStandard"));*/


	time_combo.AddString(_T("14:00"));
	time_combo.AddString(_T("14:15"));
	time_combo.AddString(_T("14:30"));
	time_combo.AddString(_T("14:45"));
	time_combo.AddString(_T("15:15"));
	time_combo.AddString(_T("15:45"));
	time_combo.AddString(_T("16:00"));
	time_combo.AddString(_T("22:40"));

	//checks
	//signature_combo.SetCurSel(0);
	//email_combo.SetCurSel(0);

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
	DDX_Control(pDX, IDC_COMBOTIME, time_combo);
}


BEGIN_MESSAGE_MAP(CMainTab, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTONSTART, &CMainTab::OnBnClickedButtonstart)
	ON_BN_CLICKED(IDC_BUTTONSTOP, &CMainTab::OnBnClickedButtonstop)
	ON_EN_CHANGE(IDC_BROWSEROOTDIRECTORY, &CMainTab::OnEnChangeBrowserootdirectory)
	ON_EN_CHANGE(IDC_BROWSEIARCHIVE, &CMainTab::OnEnChangeBrowseiarchive)
	ON_EN_CHANGE(IDC_BROWSEDNARCHIVE, &CMainTab::OnEnChangeBrowsednarchive)
	ON_CBN_SELCHANGE(IDC_COMBOTIME, &CMainTab::OnCbnSelchangeCombotime)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CMainTab message handlers

void CMainTab::LoadGeneralSettingsFromDb() {
	if (dbContext && dbContext->IsOpen()) {
		CRecordset recordset(dbContext);
		CString sqlQuery = _T("SELECT TOP 1 RootPath, InvArchPath, DnArchPath, SenderMail, SignatureId FROM GeneralSettings ORDER BY ID DESC");
		recordset.Open(CRecordset::forwardOnly, sqlQuery);

		if (!recordset.IsEOF()) {
			CString senderMail;
			CDBVariant idVariant;

			recordset.GetFieldValue(_T("RootPath"), root_directory_path);
			recordset.GetFieldValue(_T("InvArchPath"), invoice_archive_path);
			recordset.GetFieldValue(_T("DnArchPath"), dnote_archive_path);
			recordset.GetFieldValue(_T("SenderMail"), senderMail);
			recordset.GetFieldValue(_T("SignatureId"), idVariant);

			signatureId = idVariant.m_iVal;

			root_ctrl.SetWindowText(root_directory_path);
			invoice_ctrl.SetWindowText(invoice_archive_path);
			dnote_ctrl.SetWindowText(dnote_archive_path);
			email_combo.SelectString(-1, senderMail);

			//DID_IT: Figure out why select is not working. It's sorting them by alphabet
			//AddString("string") sorts by alphabet and InsertString("") is set by index
			SelectSignatureCombo(signatureId);
			//signature_combo.SelectString(-1, GetSignatureName(signatureId));
		}
		recordset.Close();
	}
}

CString CMainTab::GetSignatureId(CString name) {
	CRecordset recordset(dbContext);
	CString sqlQuery = _T("SELECT * FROM Signatures WHERE SignatureName = " + name);
	recordset.Open(CRecordset::forwardOnly, sqlQuery);

	CString value;
	if (!recordset.IsEOF()) {
		recordset.GetFieldValue(_T("ID"), value);
	}
	recordset.Close();
	return value;
}

void CMainTab::SelectSignatureCombo(int id) {

	for (int i = 0; i < signaturesList.size(); ++i)
	{
		if (signaturesList[i].id == id) {
			signature_combo.SetCurSel(i);
			return;
		}
	}
}

void CMainTab::GetAllSenderMails() {
	CRecordset recordset(dbContext);
	CString sqlQuery = _T("SELECT * FROM SenderMails");
	recordset.Open(CRecordset::forwardOnly, sqlQuery);

	while (!recordset.IsEOF()) {
		SenderEmails emailRow;

		CDBVariant idVariant;
		CString email;

		recordset.GetFieldValue(_T("Id"), idVariant);
		recordset.GetFieldValue(_T("Email"), email);

		emailRow.id = idVariant.m_iVal;
		emailRow.email = email;

		senderList.push_back(emailRow);

		recordset.MoveNext();
	}
	recordset.Close();
}

void CMainTab::GetAllSignatures() {
	CRecordset recordset(dbContext);
	CString sqlQuery = _T("SELECT * FROM Signatures");
	recordset.Open(CRecordset::forwardOnly, sqlQuery);

	while (!recordset.IsEOF()) {
		SignaturesFromDB signatureRow;

		CDBVariant idVariant;
		CString signatureName, signatureContent;

		recordset.GetFieldValue(_T("ID"), idVariant);
		recordset.GetFieldValue(_T("SignatureName"), signatureName);
		recordset.GetFieldValue(_T("Signature"), signatureContent);

		signatureRow.id = idVariant.m_iVal;
		signatureRow.SignatureName = signatureName;
		signatureRow.SignatureContent = signatureContent;

		signaturesList.push_back(signatureRow);

		recordset.MoveNext();
	}
	recordset.Close();
}

void CMainTab::OnBnClickedButtonstop()
{
	// Signal the watcher thread to stop
	if (m_hStopEvent)
	{
		SetEvent(m_hStopEvent);
	}

	// Wait for the thread to finish
	if (m_pWatcherThread)
	{
		WaitForSingleObject(m_pWatcherThread->m_hThread, INFINITE);
		m_pWatcherThread = nullptr;
		//AfxMessageBox(_T("Watcher stopped."));
		EnableControls();
	}

	// Reset stop event for future use
	if (m_hStopEvent)
	{
		ResetEvent(m_hStopEvent);
	}
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

void CMainTab::OnCbnSelchangeCombotime()
{
	int selected = -1;
	selected = time_combo.GetCurSel();
	time_combo.GetLBText(selected, time_setter);
}

CString CMainTab::GetCurrentTime()
{
	SYSTEMTIME st;
	GetLocalTime(&st);

	CString currentTime;
	currentTime.Format(_T("%02d:%02d"), st.wHour, st.wMinute);

	return currentTime;
}

void CMainTab::OnBnClickedButtonstart()
{
	// Check if the database connection is open and fetch data from controls
	if (dbContext && dbContext->IsOpen())
	{

		// Set up the stop event
		if (!m_hStopEvent)
		{
			m_hStopEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
		}

		// Start the watcher thread
		if (m_pWatcherThread == nullptr)
		{
			root_ctrl.GetWindowText(root_directory_path);
			invoice_ctrl.GetWindowText(invoice_archive_path);
			dnote_ctrl.GetWindowText(dnote_archive_path);

			CString senderMail;
			email_combo.GetWindowText(senderMail);
			OnCbnSelchangeCombotime();

			CString sqlQuery;
			sqlQuery.Format(_T("INSERT INTO GeneralSettings (RootPath, InvArchPath, DnArchPath, SenderMail, SignatureId, EndTime) ")
				_T("VALUES ('%s', '%s', '%s', '%s', '%i', '%s')"),
				root_directory_path, invoice_archive_path, dnote_archive_path, senderMail, signaturesList[signature_combo.GetCurSel()].id, time_setter);

			try
			{
				dbContext->ExecuteSQL(sqlQuery);
				AfxMessageBox(_T("Settings saved successfully!"));
			}
			catch (CDBException* e)
			{
				CString errorMsg;
				errorMsg.Format(_T("Failed to save settings: '%s'"), e->m_strError);
				AfxMessageBox(errorMsg);
				e->Delete();
			}

			DisableControls();
			m_pWatcherThread = AfxBeginThread(WatcherThreadProc, this);
			//AfxMessageBox(_T("Watcher started."));
		}
		else
		{
			AfxMessageBox(_T("Program is already running."));
		}
	}
}