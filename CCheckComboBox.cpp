#include "pch.h"
#include "CCheckComboBox.h"


CCheckComboBox::CCheckComboBox()
{
    checkboxStates.resize(10, false);
}

CCheckComboBox::~CCheckComboBox()
{
    // Clean up any resources if needed (usually nothing is required here)
}

void CCheckComboBox::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
    // Set item height (e.g., 20 pixels for each item)
    lpMeasureItemStruct->itemHeight = 20;
}

void CCheckComboBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
    CDC dc;
    dc.Attach(lpDrawItemStruct->hDC);

    // Get item data (text and checkbox state)
    CString itemText;
    GetLBText(lpDrawItemStruct->itemID, itemText);

    // Draw the background
    if (lpDrawItemStruct->itemState & ODS_SELECTED)
        dc.FillSolidRect(&lpDrawItemStruct->rcItem, RGB(0, 120, 215)); // Selected background color
    else
        dc.FillSolidRect(&lpDrawItemStruct->rcItem, RGB(255, 255, 255)); // Normal background

    // Draw the checkbox
    CRect checkBoxRect(lpDrawItemStruct->rcItem);
    checkBoxRect.right = checkBoxRect.left + 16;  // Set width for checkbox
    dc.DrawFrameControl(&checkBoxRect, DFC_BUTTON, DFCS_BUTTONCHECK);

    // Draw the text next to the checkbox
    CRect textRect(lpDrawItemStruct->rcItem);
    textRect.left = checkBoxRect.right + 2;
    dc.TextOut(textRect.left, textRect.top, itemText);

    dc.Detach();
}

void CCheckComboBox::OnLButtonDown(UINT nFlags, CPoint point)
{
    int itemIndex = GetCurSel();  // Get the selected item index
    if (itemIndex != LB_ERR)
    {
        // Toggle the checkbox state (custom logic required)
        // You can maintain a vector or array to track checkbox states
    }

    CComboBox::OnLButtonDown(nFlags, point);  // Call the default handler
}

BEGIN_MESSAGE_MAP(CCheckComboBox, CComboBox)
    // Add message handlers here if necessary
END_MESSAGE_MAP()
