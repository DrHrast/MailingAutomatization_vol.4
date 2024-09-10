// CSettingsTab.cpp : implementation file
//

#include "pch.h"
#include "NWP_project7.h"
#include "afxdialogex.h"
#include "CSettingsTab.h"
#include <iostream>
#include <stdio.h>


// CSettingsTab dialog

IMPLEMENT_DYNAMIC(CSettingsTab, CDialogEx)

CSettingsTab::CSettingsTab(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SETTINGSTAB, pParent)
{

}

CSettingsTab::~CSettingsTab()
{
}

void CSettingsTab::FIllListDialog() {
	receiverListCtrl.ResetContent();
	for (int i = 0; i < receiverList.size(); ++i) {
		receiverListCtrl.InsertString(i, receiverList[i].email);
	}
}

void CSettingsTab::PopulateReceiverList()
{
	receiverList.clear();
	CRecordset recordset(dbContext);
	CString sqlQuery = _T("SELECT * FROM ReceiverMails");
	recordset.Open(CRecordset::forwardOnly, sqlQuery);

	while (!recordset.IsEOF()) {
		ReceiverEmails emailRow;

		CDBVariant idVariant;
		CString email;

		recordset.GetFieldValue(_T("Id"), idVariant);
		recordset.GetFieldValue(_T("Email"), email);

		emailRow.id = idVariant.m_iVal;
		emailRow.email = email;

		receiverList.push_back(emailRow);

		recordset.MoveNext();
	}
	recordset.Close();
}

BOOL CSettingsTab::OnInitDialog() {
	CDialogEx::OnInitDialog();

	if (dbContext == nullptr || !dbContext->IsOpen()) {
		AfxMessageBox(_T("Database context is not set or not open in CSettingsTab!"));
		return FALSE;  // Cancel dialog if the database is not available
	}

	PopulateReceiverList();
	FIllListDialog();

	return TRUE;
}

void CSettingsTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_EDITSIGNATURENAME, signatureNameCtrl);
	DDX_Control(pDX, IDC_EDITSIGNATURECONTENT, signatureContentCtrl);
	DDX_Control(pDX, IDC_BUTTONADDSIGNATURE, addSignatureCtrl);
	DDX_Control(pDX, IDC_EDITADDSENDERMAIL, senderMailCtrl);
	DDX_Control(pDX, IDC_EDITADDRECEIVERMAIL, receiverMailCtrl);
	DDX_Control(pDX, IDC_BUTTONADDSENDERMAIL, addSenderMailCtrl);
	DDX_Control(pDX, IDC_BUTTONADDRECEIVERMAIL, addReceiverMailCtrl);
	DDX_Control(pDX, IDC_LISTRECEIVER, receiverListCtrl);
}


BEGIN_MESSAGE_MAP(CSettingsTab, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTONADDSIGNATURE, &CSettingsTab::OnBnClickedButtonaddsignature)
	ON_BN_CLICKED(IDC_BUTTONADDSENDERMAIL, &CSettingsTab::OnBnClickedButtonaddsendermail)
	ON_BN_CLICKED(IDC_BUTTONADDRECEIVERMAIL, &CSettingsTab::OnBnClickedButtonaddreceivermail)
	ON_BN_CLICKED(IDC_BUTTON4, &CSettingsTab::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CSettingsTab::OnBnClickedButton5)
END_MESSAGE_MAP()


// CSettingsTab message handlers

void CSettingsTab::OnBnClickedButtonaddsignature()
{
	CString signatureName, signatureContent;
	signatureNameCtrl.GetWindowText(signatureName);
	signatureContentCtrl.GetWindowText(signatureContent);

	if (!signatureName.IsEmpty() && !signatureContent.IsEmpty()) {
		CString sqlQuery;
		sqlQuery.Format(_T("INSERT INTO Signatures (SignatureName, Signature)")
			_T("VALUES ('%s', '%s')"),
			signatureName, signatureContent);

		try
		{
			dbContext->ExecuteSQL(sqlQuery);
			AfxMessageBox(_T("Signature saved successfully!"));
		}
		catch (CDBException* e)
		{
			AfxMessageBox(_T("Failed to save signature: ") + e->m_strError);
			e->Delete();
		}
	}
	else {
		AfxMessageBox(_T("Failed to save signature: \nFill all fields."));
	}
}


void CSettingsTab::OnBnClickedButtonaddsendermail()
{
	CString value;
	senderMailCtrl.GetWindowText(value);
	if (dbContext && dbContext->IsOpen()) {
		if (value != "") {

			CString sqlQuery;
			sqlQuery.Format(_T("INSERT INTO SenderMails (Email)")
				_T("VALUES ('%s')"),
				value);

			try
			{
				dbContext->ExecuteSQL(sqlQuery);
				AfxMessageBox(_T("New sender saved successfully!"));
			}
			catch (CDBException* e)
			{
				AfxMessageBox(_T("Failed to save sender: ") + e->m_strError);
				e->Delete();
			}
		}
		else {
			AfxMessageBox(_T("Failed to save sender: \nFill all fields."));
		}
	}

}


void CSettingsTab::OnBnClickedButtonaddreceivermail()
{
	CString value;
	receiverMailCtrl.GetWindowText(value);

	if (dbContext && dbContext->IsOpen()) {

		if (value != "") {

			CString sqlQuery;
			sqlQuery.Format(_T("INSERT INTO ReceiverMails (Email)")
				_T("VALUES ('%s')"),
				value);

			try
			{
				dbContext->ExecuteSQL(sqlQuery);
				AfxMessageBox(_T("New receiver saved successfully!"));
			}
			catch (CDBException* e)
			{
				AfxMessageBox(_T("Failed to save receiver: ") + e->m_strError);
				e->Delete();
			}
		}
		else {
			AfxMessageBox(_T("Failed to save receiver: \nFill all fields."));
		}
	}
	PopulateReceiverList();
	FIllListDialog();
}


void CSettingsTab::OnBnClickedButton4()
{
	// Get the selected index from the CListBox
	int selectedIndex = receiverListCtrl.GetCurSel();

	if (selectedIndex != LB_ERR) {  // If a valid item is selected
		CString selectedEmail;
		receiverListCtrl.GetText(selectedIndex, selectedEmail);

		int id = -1;
		for (int i = 0; i < receiverList.size(); ++i) {
			if (receiverList[i].email == selectedEmail) {
				id = receiverList[i].id;
				break;
			}
		}

		if (id != -1) {  // If a valid ID was found
			CString sqlQuery;
			sqlQuery.Format(_T("DELETE FROM ReceiverMails WHERE Id = %d"), id);

			try {
				dbContext->ExecuteSQL(sqlQuery);
				AfxMessageBox(_T("Receiver removed successfully!"));
			}
			catch (CDBException* e) {
				AfxMessageBox(_T("Failed to remove receiver: ") + e->m_strError);
				e->Delete();
			}
		}
	}
	else {
		AfxMessageBox(_T("Please select an email address\nand then click the '-' button."));
	}

	// Refresh the list after deletion
	PopulateReceiverList();
	FIllListDialog();
}



void CSettingsTab::OnBnClickedButton5()
{
	AfxMessageBox(_T("To remove receiver email address from the list\n")
		_T("select one of the shown email addresses and\n")
		_T("click '-' button."));
}
