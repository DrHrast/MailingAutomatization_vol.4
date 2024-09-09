// CListTab.cpp : implementation file
//

#include "pch.h"
#include "NWP_project7.h"
#include "afxdialogex.h"
#include "CListTab.h"


// CListTab dialog

IMPLEMENT_DYNAMIC(CListTab, CDialogEx)

CListTab::CListTab(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_LISTTAB, pParent)
{

}

CListTab::~CListTab()
{
}

BOOL CListTab::OnInitDialog() {
	CDialogEx::OnInitDialog();

	FillListDialog();

	return TRUE;
}

void CListTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LISTBUYERS, buyerList);
}

void CListTab::FillListDialog() {
	CRecordset recordset(dbContext);
	CString sqlQuery = _T("SELECT Vat, Name, Primary_email, Secondary_email FROM Buyers");
	recordset.Open(CRecordset::forwardOnly, sqlQuery);
	
	buyerList.InsertColumn(0, _T("VAT"), LVCFMT_LEFT, 100);
	buyerList.InsertColumn(1, _T("Name"), LVCFMT_LEFT, 150);
	buyerList.InsertColumn(2, _T("Primary email"), LVCFMT_LEFT, 200);
	buyerList.InsertColumn(3, _T("Secondary email"), LVCFMT_LEFT, 250);

	int index = 0;

	while (!recordset.IsEOF()) {
		CString vat, name, primEmail, secEmail;

		recordset.GetFieldValue(_T("Vat"), vat);
		recordset.GetFieldValue(_T("Name"), name);
		recordset.GetFieldValue(_T("Primary_email"), primEmail);
		recordset.GetFieldValue(_T("Secondary_email"), secEmail);

		buyerList.InsertItem(index, vat);
		buyerList.SetItemText(index, 1, name);
		buyerList.SetItemText(index, 2, primEmail);
		buyerList.SetItemText(index, 3, secEmail);

		recordset.MoveNext();
		index++;
	}
	recordset.Close();
}

void CListTab::AddNewUserToDb() {
	CString vat, primaryEmail, secondaryEmail, shortName, name;
	//TODO: something ain't right
	addVatCtrl.GetWindowText(vat);
	addShortNameCtrl.GetWindowText(shortName);
	addNameCtrl.GetWindowText(name);
	addPrimaryMailCtrl.GetWindowText(primaryEmail);
	addSecondaryMailCtrl.GetWindowText(secondaryEmail);

	vat.Replace(_T("'"), _T("''"));
	primaryEmail.Replace(_T("'"), _T("''"));
	secondaryEmail.Replace(_T("'"), _T("''"));
	shortName.Replace(_T("'"), _T("''"));
	name.Replace(_T("'"), _T("''"));

	CString sqlQuery;
	sqlQuery.Format(_T("INSERT INTO Buyers (Vat, ShortName, Name, Primary_email, Secondary_email) ")
		_T("VALUES ('%s', '%s', '%s', '%s', '%s')"),
		vat, shortName, name, primaryEmail, secondaryEmail);

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

void CListTab::ClearFields() {
	addVatCtrl.SetWindowText(_T(""));
	addShortNameCtrl.SetWindowText(_T(""));
	addNameCtrl.SetWindowText(_T(""));
	addPrimaryMailCtrl.SetWindowText(_T(""));
	addSecondaryMailCtrl.SetWindowText(_T(""));
}


BEGIN_MESSAGE_MAP(CListTab, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTONADDADD, &CListTab::OnBnClickedButtonaddadd)
	ON_BN_CLICKED(IDC_BUTTONADDCLEAR, &CListTab::OnBnClickedButtonaddclear)
END_MESSAGE_MAP()


// CListTab message handlers


void CListTab::OnBnClickedButtonaddadd()
{
	AddNewUserToDb();
	ClearFields();
	FillListDialog();
}


void CListTab::OnBnClickedButtonaddclear()
{
	ClearFields();
}
