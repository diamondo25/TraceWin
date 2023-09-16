////////////////////////////////////////////////////////////////
// TRACEWIN Copyright 1997-2003 Paul DiLascia
// If this code works, it was written by Paul DiLascia.
// If not, I don't know who wrote it.
//
// Implementation of TRACEWIN applet. This is the applet sits around
// and waiting for WM_COPYDATA messages from other apps using TRACEWIN.
//
#include "StdAfx.h"
#include "MainFrm.h"
#include "resource.h"
#include "WinPlace.h"
#include "StatLink.h"
#include "MyVer.h"
#include "WebVersion.h"

/////////////////
// Application class
//
class CTraceWinApp : public CWinApp
{
public:
	DWORD m_dwNewVersion;
	CTraceWinApp();
	~CTraceWinApp();
	virtual BOOL InitInstance();
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
} App;

BEGIN_MESSAGE_MAP(CTraceWinApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
END_MESSAGE_MAP()

CTraceWinApp::CTraceWinApp() : m_dwNewVersion(0)
{
}

CTraceWinApp::~CTraceWinApp()
{
}

BOOL CTraceWinApp::InitInstance()
{
	CWnd* pWnd = CWnd::FindWindow(_T(TRACEWND_CLASSNAME), NULL);
	if (pWnd) {
		pWnd->SetForegroundWindow();
		return FALSE;
	}

	// Save settings in registry, not INI file
	SetRegistryKey(_T("TraceWin"));

   // Create main frame window (don't use doc/view)
   CMainFrame* pMainFrame = new CMainFrame;
   if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
      return FALSE;

	// Load window position from profile
	CWindowPlacement wp;
	if (!wp.Restore(CMainFrame::REGKEY, _T("MainWindow"), pMainFrame))
		pMainFrame->ShowWindow(m_nCmdShow);
   pMainFrame->UpdateWindow();
   m_pMainWnd = pMainFrame;

	// read current version number from web
	m_dwNewVersion = 0; // assume failure
#if 0
	if (CWebVersion::Online()) {
		CWebVersion ver(_T("ftp.dilascia.com"));
		if (ver.ReadVersion(_T("TraceWinVer")))
			m_dwNewVersion = ver.dwVersionMS;
	}
#endif

	return TRUE;
}

//////////////////
// Custom about dialog uses CStaticLink for hyperlinks.
//    * for text control, URL is specified as text in dialog editor
//    * for icon control, URL is specified by setting m_iconLink.m_link
//
class CAboutDialog : public CDialog {
protected:
	// static controls with hyperlinks
	CStaticLink	m_wndLink1;
	CStaticLink	m_wndLink2;
	CStaticLink	m_wndLink3;
	CStatic m_wndVersion;
public:
	CAboutDialog() : CDialog(IDD_ABOUTBOX) { }
	virtual BOOL OnInitDialog();
};

BOOL CAboutDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// subclass static controls.
	m_wndLink1.SubclassDlgItem(IDC_URLTEXT,this);
	m_wndLink2.SubclassDlgItem(IDC_URLICON,this);
	m_wndLink3.SubclassDlgItem(IDC_URLUPDATE,this);
	m_wndVersion.SubclassDlgItem(IDC_VERSION,this);

	// Read my own version info and set static text
	CMyVersionInfo vi;
	CString s;
	s.Format(_T("TraceWin Version %d.%02d by Paul DiLascia"),
		HIWORD(vi.dwProductVersionMS),LOWORD(vi.dwProductVersionMS));
	m_wndVersion.SetWindowText(s);

	// compare my version to current version on web site
	if (App.m_dwNewVersion && vi.dwProductVersionMS < App.m_dwNewVersion) {
		// current version is newer: display link
		s.Format(_T("Get version %d.%02d"),
			HIWORD(App.m_dwNewVersion),LOWORD(App.m_dwNewVersion));
		m_wndLink3.SetWindowText(s);

	} else {
		// current version not available: hide link
		m_wndLink3.ShowWindow(SW_HIDE);
	}
	return TRUE;
}

//////////////////
// Handle Help | About : run the About dialog
//
void CTraceWinApp::OnAppAbout()
{
	static CAboutDialog dlg;
	dlg.DoModal();
}
