////////////////////////////////////////////////////////////////
// TRACEWIN Copyright 1997-2003 Paul DiLascia
// If this code works, it was written by Paul DiLascia.
// If not, I don't know who wrote it.
//

//////////////////
// Buffer window for TRACEWIN.
// CBufWnd implements the trace buffer. 
// Implements its own editing commands commands: Copy, Clear, Select All. 
// In order to work, the CBufWnd must be connected to the app's command 
// route by overriding CMainFrame::OnCmdMsg.
//
class CBufWnd : public CEdit {
public:
	CBufWnd();
	virtual ~CBufWnd();
	bool Create(CWnd* pParentWnd, UINT nID);
	bool IsEmpty();
protected:
	afx_msg void OnEditClearAll();
	afx_msg void OnEditCopy();
	afx_msg void OnEditSelectAll();
	afx_msg void OnUpdateEditClearAll(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditSelectAll(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()
};
