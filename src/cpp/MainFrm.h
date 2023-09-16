////////////////////////////////////////////////////////////////
// TRACEWIN Copyright 1997-2003 Paul DiLascia
// If this code works, it was written by Paul DiLascia.
// If not, I don't know who wrote it.
//
#include "BufWnd.h"

// Window class name used by the main window of the TRACEWIN applet.
#define TRACEWND_CLASSNAME "MfxTraceWindow"

// ID sent as COPYDATASRUCT::dwData to identify the WM_COPYDATA message
// as coming from an app using TRACEWIN.
#define ID_COPYDATA_TRACEMSGA MAKELONG(MAKEWORD('t','w'),MAKEWORD('i','n'))
#define ID_COPYDATA_TRACEMSGW (ID_COPYDATA_TRACEMSGA+1)

/////////////////
// Main frame window for TRACEWIN contains trace buffer as child window, 
// not a view.
//
class CMainFrame : public CFrameWnd {
	DECLARE_DYNAMIC(CMainFrame)
	CBufWnd		m_wndBuffer;		// TRACE buffer
	CFile			m_file;				// if outputting to file
	UINT			m_nOutputWhere;	// where to output
	CFont			m_font;				// display font
	bool			m_bRotateBuf;		// delete old stuff when full
	UINT			m_nLimitText;		// max buffer text size

	// Helpers
	bool OpenFile(CFile& f, LPCTSTR lpszPathName);	// helper
	void CloseTraceFile();

public:
	CMainFrame();
	virtual ~CMainFrame();
	static LPCTSTR REGKEY;	// registry key for saved settings

protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra,
		AFX_CMDHANDLERINFO* pHandlerInfo);

	//{{AFX_MSG(CMainFrame)
	afx_msg LRESULT OnTraceMsg(WPARAM wp, LPARAM lp);
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnClose();

	afx_msg void OnFileSaveAs();
	afx_msg void OnUpdateFileSaveAs(CCmdUI* pCmdUI);
	afx_msg void OnOutputOff();
	afx_msg void OnUpdateOutputOff(CCmdUI* pCmdUI);
	afx_msg void OnOutputToFile();
	afx_msg void OnUpdateOutputToFile(CCmdUI* pCmdUI);
	afx_msg void OnOutputToWindow();
	afx_msg void OnUpdateOutputToWindow(CCmdUI* pCmdUI);
	afx_msg void OnKeepOnTop();
	afx_msg void OnUpdateKeepOnTop(CCmdUI* pCmdUI);
	afx_msg void OnFontChange();
	afx_msg void OnRotatingBuf();
	afx_msg void OnUpdateRotatingBuf(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
