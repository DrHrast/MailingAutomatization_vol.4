#include "pch.h"
#include "NWP_project7.h"
#include "afxdialogex.h"
#include "ReceiverCheckComboBox.h"

// ReceiverCheckComboBox dialog

IMPLEMENT_DYNAMIC(ReceiverCheckComboBox, CDialogEx)

ReceiverCheckComboBox::ReceiverCheckComboBox(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDC_CHECKCOMBOBOXRECEIVER, pParent)
{
}

void ReceiverCheckComboBox::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    // Bind the custom combo box to the dialog resource (IDC_CHECKCOMBOBOXRECEIVER is the resource ID)
    DDX_Control(pDX, IDC_CHECKCOMBOBOXRECEIVER, m_receiverCheckComboBox);
}

BEGIN_MESSAGE_MAP(ReceiverCheckComboBox, CDialogEx)
    // Add any message handlers here
END_MESSAGE_MAP()

BOOL ReceiverCheckComboBox::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // Add items to the custom combo box with checkboxes
    m_receiverCheckComboBox.AddString(_T("Receiver 1"));
    m_receiverCheckComboBox.AddString(_T("Receiver 2"));
    m_receiverCheckComboBox.AddString(_T("Receiver 3"));

    return TRUE;  // return TRUE unless you set the focus to a control
}
