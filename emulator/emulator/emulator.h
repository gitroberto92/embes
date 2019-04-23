
// emulator.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols
#include "emulatorDlg.h"

// CemulatorApp:
// See emulator.cpp for the implementation of this class
//

class CemulatorApp : public CWinApp
{
public:
	CemulatorApp();

private:
	CemulatorDlg* m_dlg;
	CString m_keystr;
// Overrides
public:
	virtual BOOL InitInstance();
	ULONG_PTR m_nGdiplusToken;

// Implementation

	DECLARE_MESSAGE_MAP()
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

extern CemulatorApp theApp;