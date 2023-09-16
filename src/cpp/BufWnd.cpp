////////////////////////////////////////////////////////////////
// TRACEWIN Copyright 1997-2003 Paul DiLascia
// If this code works, it was written by Paul DiLascia.
// If not, I don't know who wrote it.
//

////////////////////////////////////////////////////////////////
// Trace buffer class for TRACEWND applet.
//
#include "StdAfx.h"
#include "BufWnd.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CBufWnd, CEdit)
	//{{AFX_MSG_MAP(CBufWnd)
	ON_COMMAND(ID_EDIT_CLEAR_ALL, OnEditClearAll)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_SELECT_ALL, OnEditSelectAll)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CLEAR_ALL, OnUpdateEditClearAll)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_SELECT_ALL, OnUpdateEditSelectAll)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CBufWnd::CBufWnd()
{
}

CBufWnd::~CBufWnd()
{
}

//////////////////
// Create a multiline edit control
//
BOOL CBufWnd::Create(CWnd* pParentWnd, UINT nID)
{
	CRect rect;
	return CEdit::Create(
		WS_VISIBLE | WS_CHILD | WS_VSCROLL | WS_HSCROLL | ES_MULTILINE,
		rect, 
		pParentWnd, 
		nID);
}

/////////////////
// Helper returns TRUE of buffer is empty
//
BOOL CBufWnd::IsEmpty() 
{
	return GetLineCount() <= 1 && LineLength() == 0;
}

void CBufWnd::OnEditClearAll() 
{
	OnEditSelectAll();		// select all...
	Clear();						// ..then delete
}

void CBufWnd::OnUpdateEditClearAll(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(!IsEmpty());	// enabled if not empty
}

void CBufWnd::OnEditCopy() 
{
	Copy();						// duh
}

void CBufWnd::OnUpdateEditCopy(CCmdUI* pCmdUI) 
{
	// Enabled if something selected
	int begin, end;
	GetSel(begin, end);
	pCmdUI->Enable(begin != end);
}

void CBufWnd::OnEditSelectAll() 
{
	SetSel(0, -1);		//  select whole buffer
}

void CBufWnd::OnUpdateEditSelectAll(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!IsEmpty());	// enabled if not empty
}

