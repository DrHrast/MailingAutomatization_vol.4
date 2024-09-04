
// NWP_project7.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CNWPproject7App:
// See NWP_project7.cpp for the implementation of this class
//

class CNWPproject7App : public CWinApp
{
public:
	CNWPproject7App();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CNWPproject7App theApp;
