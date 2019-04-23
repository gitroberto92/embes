
// emulator.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "emulator.h"
#include "emulatorDlg.h"

#include "engine_main.h"
#include "ConfigFileManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const char g_states[][14] = {
	"11111111111",
	"11111111100",
	"11111110000",
	"11111011100",
	"11111000000",
	"11110000000",
	"11100000000",
	"11011000000",
	"11010000000",
	"00010000000",
	"01111110000",
	"01111010000",
	"01111000000",
	"01110000000",
	"01110110000",
	"01100110000",
	"01101101100",
	"11111111110",
	"11111111000",
	"11111101111",
	"11110111000",
	"11101100000",
	"00110000000",
	"01111001100",
	"01111001100",
	"01110100000",
	"01110111100",
	"01101110010"
};


// CemulatorApp

BEGIN_MESSAGE_MAP(CemulatorApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CemulatorApp construction

CemulatorApp::CemulatorApp()
{
	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CemulatorApp object

CemulatorApp theApp;


// CemulatorApp initialization

BOOL CemulatorApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	Gdiplus::GdiplusStartup(&m_nGdiplusToken, &gdiplusStartupInput, NULL);

	// Create the shell manager, in case the dialog contains
	// any shell tree view or shell list view controls.
	CShellManager *pShellManager = new CShellManager;

	// Activate "Windows Native" visual manager for enabling themes in MFC controls
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	CemulatorDlg dlg;
	m_dlg = &dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "Warning: dialog creation failed, so application is terminating unexpectedly.\n");
		TRACE(traceAppMsg, 0, "Warning: if you are using MFC controls on the dialog, you cannot #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS.\n");
	}

	// Delete the shell manager created above.
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

#ifndef _AFXDLL
	ControlBarCleanUp();
#endif

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}


BOOL CemulatorApp::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class

	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case VK_UP:
			increaseVolumn(0.01f);
			break;
		case VK_DOWN:
			increaseVolumn(-0.01f);
			break;
		case VK_LEFT:
			setVolumn(0);
			break;
		case VK_RIGHT:
			setVolumn(1);
			break;

		default:
			m_dlg->sendKeyToEngine(pMsg->wParam, NOTE_ON);
			break;
		}
	}

	else if (pMsg->message == WM_KEYUP) {
		m_dlg->sendKeyToEngine(pMsg->wParam, NOTE_OFF);
		
	}
	m_dlg->m_slVol.SetCurrValue((double)getVolumn(), true);
	return CWinApp::PreTranslateMessage(pMsg);
}

