#pragma once
#include "afxdialogex.h"
#include "CCheckComboBox.h"

// ReceiverCheckComboBox dialog
class ReceiverCheckComboBox : public CDialogEx
{
    DECLARE_DYNAMIC(ReceiverCheckComboBox)  // Adds runtime dynamic type information

public:
    ReceiverCheckComboBox(CWnd* pParent = nullptr);   // Standard constructor

    // Custom Combo Box for Checkboxes
    CCheckComboBox m_receiverCheckComboBox;

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual BOOL OnInitDialog();  // Initialization

    DECLARE_MESSAGE_MAP()
};
