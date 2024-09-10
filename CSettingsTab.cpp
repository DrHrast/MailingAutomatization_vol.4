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

BOOL CSettingsTab::OnInitDialog() {

	CDialogEx::OnInitDialog();

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
}


BEGIN_MESSAGE_MAP(CSettingsTab, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTONADDSIGNATURE, &CSettingsTab::OnBnClickedButtonaddsignature)
	ON_BN_CLICKED(IDC_BUTTONADDSENDERMAIL, &CSettingsTab::OnBnClickedButtonaddsendermail)
	ON_BN_CLICKED(IDC_BUTTONADDRECEIVERMAIL, &CSettingsTab::OnBnClickedButtonaddreceivermail)
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
}
