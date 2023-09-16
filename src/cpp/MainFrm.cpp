////////////////////////////////////////////////////////////////
// TRACEWIN Copyright 1997-2003 Paul DiLascia
// If this code works, it was written by Paul DiLascia.
// If not, I don't know who wrote it.
//
// Main window for TRACEWIN
//
#include "StdAfx.h"
#include "MainFrm.h"
#include "resource.h"
#include "WinPlace.h"
#include "FontUI.h"
#include <direct.h>

// Registry key for profile settings
LPCTSTR CMainFrame::REGKEY = _T("Settings");

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)
BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_MESSAGE(WM_COPYDATA, OnTraceMsg)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_COMMAND(ID_FILE_SAVE_AS,					OnFileSaveAs)
	ON_COMMAND(ID_OUTPUT_OFF,						OnOutputOff)
	ON_COMMAND(ID_OUTPUT_TO_FILE,					OnOutputToFile)
	ON_COMMAND(ID_OUTPUT_TO_WINDOW,				OnOutputToWindow)
	ON_COMMAND(ID_KEEP_ON_TOP,						OnKeepOnTop)
	ON_COMMAND(ID_EDIT_FONT,						OnFontChange)
	ON_COMMAND(ID_ROTATING_BUF,					OnRotatingBuf)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_AS,		OnUpdateFileSaveAs)
	ON_UPDATE_COMMAND_UI(ID_OUTPUT_OFF,			OnUpdateOutputOff)
	ON_UPDATE_COMMAND_UI(ID_OUTPUT_TO_FILE,	OnUpdateOutputToFile)
	ON_UPDATE_COMMAND_UI(ID_OUTPUT_TO_WINDOW, OnUpdateOutputToWindow)
	ON_UPDATE_COMMAND_UI(ID_KEEP_ON_TOP,		OnUpdateKeepOnTop)
	ON_UPDATE_COMMAND_UI(ID_ROTATING_BUF,		OnUpdateRotatingBuf)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CMainFrame::CMainFrame() 
{
	// Load state from profile
	m_nOutputWhere = ID_OUTPUT_TO_WINDOW;
	CString dir  = AfxGetApp()->GetProfileString(REGKEY,_T("Directory"));
	m_bRotateBuf = AfxGetApp()->GetProfileInt(REGKEY, _T("RotatingBuf"), 1);
	m_nLimitText = 0;
	if (!dir.IsEmpty())
		UNUSED(_tchdir(dir));
}

CMainFrame::~CMainFrame()
{
}

/////////////////
// Override to use "MfxTraceWindow" as 
// class name so applications can find us.
//
BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	static LPCTSTR className = NULL;

	if (!CFrameWnd::PreCreateWindow(cs))
		return FALSE;

	if (className==NULL) {
		// One-time class registration
		// The only purpose is to make the class name something meaningful
		// instead of "Afx:0x4d:27:32:hup1hup:hike!"
		//
		WNDCLASS wndcls;
		::GetClassInfo(AfxGetInstanceHandle(), cs.lpszClass, &wndcls);
		wndcls.lpszClassName = _T(TRACEWND_CLASSNAME);
		wndcls.hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
		VERIFY(AfxRegisterClass(&wndcls));
		className=_T(TRACEWND_CLASSNAME);
	}
	cs.lpszClass = className;
	if (AfxGetApp()->GetProfileInt(REGKEY, _T("KeepOnTop"), 0))
		cs.dwExStyle |= WS_EX_TOPMOST;
	return TRUE;
}

//////////////////
// Main frame created.
//
int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Create buffer window
	if (!m_wndBuffer.Create(this, AFX_IDW_PANE_FIRST)) {
		TRACE(_T("Failed to create buffer window\n"));
		return -1;      // fail to create
	}

	// Restore font from profile
	CFontUI fui;
	if (!fui.GetProfileFont(REGKEY, _T("Font"), m_font)) {
		// Use 8pt Courier (monospace) default
		m_font.CreatePointFont(80,_T("Courier"));
	}
	m_wndBuffer.SetFont(&m_font);
	m_nLimitText = m_wndBuffer.GetLimitText();

	return 0;
}

//////////////////
// If I get focus, give to buffer
//
void CMainFrame::OnSetFocus(CWnd* pOldWnd)
{
	m_wndBuffer.SetFocus();
}

//////////////////
// Got command message: pass to trace buffer
// Without this, m_wndBuffer doesn't get a chance to handle 
// menu commands with ON_COMMAND, etc.
//
BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra,
	AFX_CMDHANDLERINFO* pHandlerInfo)
{
	if (m_wndBuffer.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;	// handled by buffer window

	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

//////////////////
// Someone sent me a WM_COPYDATA message: display it
//
LRESULT CMainFrame::OnTraceMsg(WPARAM wParam, LPARAM lParam)
{
	COPYDATASTRUCT* pcds = (COPYDATASTRUCT*)lParam;
	if (m_nOutputWhere==ID_OUTPUT_OFF)
		return 0;

	LPCWSTR lpText=NULL;
	UINT len = pcds->cbData;
	UINT nchars = 0;
	static TCHAR buf[1024] = {0};

	if (pcds->dwData==ID_COPYDATA_TRACEMSGW) {
		lpText = (LPCWSTR)pcds->lpData;		 // already Unicode
		nchars = len/sizeof(WCHAR);

	} else if (pcds->dwData==ID_COPYDATA_TRACEMSGA) {
		// convert ANSI to Unicode
		WCHAR buf[1024];
		nchars = MultiByteToWideChar(CP_ACP,0,
			(LPCSTR)pcds->lpData,len,buf,sizeof(buf)/sizeof(buf[0]));
		lpText = buf;
		
	} else {
		return 0; // submessage not recognized
	}

	if (m_nOutputWhere==ID_OUTPUT_TO_WINDOW) {

		// check for clear-screen char (Ctl-L)
		if (*lpText == _T('')) {
			m_wndBuffer.SetSel(0, -1);
			m_wndBuffer.SendMessage(WM_CLEAR);
			lpText++;
			nchars--;
		}
	
		// Convert \n to \n\r for Windows brain-damaged edit control
		// It's 1995, and I'm still writing code like this!
		//
		WCHAR buf[1024] = {0};
		WCHAR* endbuf = buf + sizeof(buf)/sizeof(buf[0]);
		WCHAR* dst = buf;
		LPCWSTR src = lpText;

		while (nchars-- > 0 && dst < endbuf) {
			if (*src == _T('\n'))
				*dst++ = _T('\r');
			*dst++ = *src++;
		}
		*dst = 0;

		// Append string to contents of trace buffer
		int iLen = m_wndBuffer.GetWindowTextLength();
		if (iLen + len >= m_nLimitText) {
			if (m_bRotateBuf) {
				m_wndBuffer.SetSel(0, m_nLimitText/4);
				m_wndBuffer.ReplaceSel(_T(""));
				iLen = m_wndBuffer.GetWindowTextLength();
			} else
				iLen =-1;
		}
		if (iLen>=0) {
			m_wndBuffer.SetSel(iLen, iLen);	 // end of edit text
			m_wndBuffer.ReplaceSel(buf);		 // append string..
			m_wndBuffer.SendMessage(EM_SCROLLCARET);	// ..and show caret
		}

	} else if (m_nOutputWhere==ID_OUTPUT_TO_FILE) {
		m_file.Write(lpText, len);
	}
	return TRUE;
}

//////////////////
// Save contents of trace buffer to a file
//
void CMainFrame::OnFileSaveAs() 
{
	CFileDialog dlg(FALSE);
	if (dlg.DoModal()!=IDOK) 
		return;

	CFile f;
	if (OpenFile(f, dlg.GetPathName())) {
		WCHAR buf[256];
		int count = m_wndBuffer.GetLineCount();
		for (int line=0; line<count; line++) {
			int len = m_wndBuffer.GetLine(line, buf, ARRAYSIZE(buf)-2);
			buf[len++]=_T('\n');
			f.Write(buf, len);
		}
		f.Close();
	}
}

//////////////////
// Helper to open a file
//
bool CMainFrame::OpenFile(CFile& f, LPCWSTR lpszPathName)
{
	BOOL bOpen = FALSE;
	TRY {
		bOpen = f.Open(lpszPathName,
			CFile::modeCreate | CFile::modeWrite | CFile::shareDenyWrite);

	} CATCH_ALL(e) {
	} END_CATCH_ALL

	if (!bOpen) {
		MessageBox(_T("Failed to open file."),
			_T("TRACEWIN"), MB_OK|MB_ICONEXCLAMATION);
	}

	return bOpen == TRUE;
}

void CMainFrame::CloseTraceFile()
{
	if (m_file.m_hFile != CFile::hFileNull)
		m_file.Close();
}

void CMainFrame::OnUpdateFileSaveAs(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!m_wndBuffer.IsEmpty());
}

void CMainFrame::OnOutputOff() 
{
	CloseTraceFile();
	m_nOutputWhere = ID_OUTPUT_OFF;
}

void CMainFrame::OnUpdateOutputOff(CCmdUI* pCmdUI) 
{
	pCmdUI->SetRadio(m_nOutputWhere==ID_OUTPUT_OFF);
}

void CMainFrame::OnOutputToFile() 
{
	CFileDialog dlg(TRUE);
	dlg.m_ofn.lpstrTitle = _T("Output to File");
	if (dlg.DoModal()!=IDOK) 
		return;

	CloseTraceFile();
	if (OpenFile(m_file, dlg.GetPathName()))
		m_nOutputWhere = ID_OUTPUT_TO_FILE;
}

void CMainFrame::OnUpdateOutputToFile(CCmdUI* pCmdUI) 
{
	pCmdUI->SetRadio(m_nOutputWhere==ID_OUTPUT_TO_FILE);
}

void CMainFrame::OnOutputToWindow() 
{
	CloseTraceFile();
	m_nOutputWhere = ID_OUTPUT_TO_WINDOW;
}

void CMainFrame::OnUpdateOutputToWindow(CCmdUI* pCmdUI) 
{
	pCmdUI->SetRadio(m_nOutputWhere==ID_OUTPUT_TO_WINDOW);
}

void CMainFrame::OnKeepOnTop() 
{
	DWORD dwStyle = ::GetWindowLong(m_hWnd, GWL_EXSTYLE);
	const CWnd* pWnd = dwStyle & WS_EX_TOPMOST ? &wndNoTopMost : &wndTopMost;
	SetWindowPos(pWnd,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);
}

void CMainFrame::OnUpdateKeepOnTop(CCmdUI* pCmdUI) 
{
	DWORD dwStyle = ::GetWindowLong(m_hWnd, GWL_EXSTYLE);
	pCmdUI->SetCheck(dwStyle & WS_EX_TOPMOST ? TRUE : FALSE);
}

//////////////////
// Frame window closed: save position, size in profile.
//
void CMainFrame::OnClose() 
{
	// Save current settings in registry
	CWindowPlacement wp;
	wp.Save(REGKEY, _T("MainWindow"), this);

	CWinApp *pApp = AfxGetApp();
	pApp->WriteProfileInt(REGKEY, _T("TraceWhere"),  m_nOutputWhere);
	pApp->WriteProfileInt(REGKEY, _T("RotatingBuf"), m_bRotateBuf);
	pApp->WriteProfileInt(REGKEY, _T("KeepOnTop"),  
		::GetWindowLong(m_hWnd, GWL_EXSTYLE) & WS_EX_TOPMOST ? TRUE : FALSE);

	TCHAR buf[_MAX_PATH] = {0};
	UNUSED(_tgetcwd(buf, ARRAYSIZE(buf)));
	pApp->WriteProfileString(REGKEY, _T("Directory"), buf);

	CFontUI().WriteProfileFont(REGKEY, _T("Font"), m_font);

	CFrameWnd::OnClose();
}

////////////////////////////////////////////////////////////////
// Handle font change (bigger/smaller/dialog)
//
void CMainFrame::OnFontChange()
{
	CFontUI fui;
	if (fui.OnChangeFont(m_font,0,this,
		CF_SCREENFONTS|CF_FORCEFONTEXIST|CF_FIXEDPITCHONLY)) {
		m_wndBuffer.SetFont(&m_font);
		m_wndBuffer.Invalidate(TRUE);
		m_wndBuffer.UpdateWindow();
	}
}

void CMainFrame::OnRotatingBuf()
{
	m_bRotateBuf = !m_bRotateBuf;
}

void CMainFrame::OnUpdateRotatingBuf(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_bRotateBuf);
}
