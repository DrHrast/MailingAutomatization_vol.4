#pragma once
#include <afxwin.h>
#include <vector>


class CCheckComboBox : public CComboBox
{
public:
    CCheckComboBox();
    virtual ~CCheckComboBox();

protected:
    virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
    virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

    std::vector<bool> checkboxStates;

    DECLARE_MESSAGE_MAP()
};

