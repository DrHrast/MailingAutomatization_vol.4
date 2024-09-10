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
	DDX_Control(pDX, IDC_EDITADDVAT, addVatCtrl);
	DDX_Control(pDX, IDC_EDITADDNAME, addNameCtrl);
	DDX_Control(pDX, IDC_EDITADDSNAME, addShortNameCtrl);
	DDX_Control(pDX, IDC_EDITADDPRIM, addPrimaryMailCtrl);
	DDX_Control(pDX, IDC_EDITADDSEC, addSecondaryMailCtrl);
	DDX_Control(pDX, IDC_EDITUPVAT, updateVatCtrl);
	DDX_Control(pDX, IDC_EDITUPPRIM, updatePrimaryMailCtrl);
	DDX_Control(pDX, IDC_EDITUPSEC, updateSecondaryMailCtrl);
}

void CListTab::FillListDialog() {
	buyerList.DeleteAllItems();
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

void CListTab::AddNewBuyerToDb() {
	CString vat, primaryEmail, secondaryEmail, shortName, name;
	//TODO: something ain't right
	addVatCtrl.GetWindowText(vat);
	addShortNameCtrl.GetWindowText(shortName);
	addNameCtrl.GetWindowText(name);
	addPrimaryMailCtrl.GetWindowText(primaryEmail);
	addSecondaryMailCtrl.GetWindowText(secondaryEmail);

	/*vat.Replace(_T("'"), _T("''"));
	primaryEmail.Replace(_T("'"), _T("''"));
	secondaryEmail.Replace(_T("'"), _T("''"));
	shortName.Replace(_T("'"), _T("''"));
	name.Replace(_T("'"), _T("''"));*/

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

void CListTab::UpdateBuyer() {
	CString vat, primaryEmail, secondaryEmail;
	updateVatCtrl.GetWindowText(vat);
	updatePrimaryMailCtrl.GetWindowText(primaryEmail);
	updateSecondaryMailCtrl.GetWindowText(secondaryEmail);

	CString sqlQuery;
	sqlQuery.Format(_T("UPDATE Buyers SET Primary_email = '%s', Secondary_email = '%s' ")
		_T("WHERE Vat = '%s'"),
		primaryEmail, secondaryEmail, vat);

	try
	{
		dbContext->ExecuteSQL(sqlQuery);
		AfxMessageBox(_T("Settings updated successfully!"));
	}
	catch (CDBException* e)
	{
		AfxMessageBox(_T("Failed to update settings: ") + e->m_strError);
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
	ON_BN_CLICKED(IDC_BUTTONUPCLEAR, &CListTab::OnBnClickedButtonupclear)
	ON_BN_CLICKED(IDC_BUTTONUPADD, &CListTab::OnBnClickedButtonupadd)
END_MESSAGE_MAP()


// CListTab message handlers


void CListTab::OnBnClickedButtonaddadd()
{
	AddNewBuyerToDb();
	ClearFields();
	FillListDialog();
}


void CListTab::OnBnClickedButtonaddclear()
{
	ClearFields();
}


void CListTab::OnBnClickedButtonupadd()
{
	UpdateBuyer();
	ClearFields();
	FillListDialog();
}


void CListTab::OnBnClickedButtonupclear()
{
	ClearFields();
}
