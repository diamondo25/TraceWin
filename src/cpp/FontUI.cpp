////////////////////////////////////////////////////////////////
// PixieLib(TM) Copyright 1997-1999 Paul DiLascia
// If this code works, it was written by Paul DiLascia.
// If not, I don't know who wrote it.
//
// CFontUI handles the user interface for changing font sizes, as well
// as saving/restoring font info in the application profile.
//
#include "StdAfx.h"
#include "FontUI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CFontUI::CFontUI()
{
	m_nFontPtSizeMin = 4;
	m_nFontPtSizeMax = 120;
}

CFontUI::~CFontUI()
{
}

//////////////////
// Get font point size. Convert device units to points.
// There are 72 points per inch.
//
int CFontUI::GetFontPointSize(CFont& font, CDC& dc)
{
	LOGFONT lf;
	font.GetLogFont(&lf);
	return MulDiv(-lf.lfHeight, 72, dc.GetDeviceCaps(LOGPIXELSY));	
}

//////////////////
// Set font point size. Convert points to device units
// There are 72 points per inch.
//
BOOL CFontUI::SetFontPointSize(CFont& font, CDC& dc, int pts)
{
	LOGFONT lf;
	font.GetLogFont(&lf);
	lf.lfHeight = MulDiv(-pts, dc.GetDeviceCaps(LOGPIXELSY), 72);
	font.DeleteObject();
	return font.CreateFontIndirect(&lf);	
}

//////////////////
// Main UI function.
//		op = 0  ==>	run common font dialog
//    op < 0  ==>	font size smaller
//    op < 0  ==>	font size bigger
//
// Returns BOOL, whether changed or not, and CFont has new font.
//
BOOL CFontUI::OnChangeFont(CFont& font, int op, CWnd* pWnd, DWORD dwFlags)
{
	ASSERT(font.m_hObject);
	if (op==0) {
		// Run common font dialog
		LOGFONT logfont;
		font.GetLogFont(&logfont);
		CFontDialog dlg(&logfont, dwFlags, NULL, pWnd);
		dlg.m_cf.nSizeMin = m_nFontPtSizeMin;
		dlg.m_cf.nSizeMax = m_nFontPtSizeMax;
		if (dlg.DoModal() != IDOK)
			return FALSE;
		// Change the font
		font.DeleteObject();
		return font.CreateFontIndirect(&logfont);

	}
	// Grow or shrink
	CWindowDC dc(NULL);						  // use screen DC
	int pts = GetFontPointSize(font, dc); // get point size
	pts = GrowFontSize(pts, op);			  // grow (or shrink)
	if (pts < m_nFontPtSizeMin  || pts > m_nFontPtSizeMax)
		return FALSE;
	return SetFontPointSize(font, dc, pts);
}

//////////////////
// Increment or decrement font point size based on current size.
// Algorithm:
// If point size is
//			<= 12		incr = 1 pts
//			10..32	incr = 2 pts
//			32..48   incr = 4 pts
//			>= 48    incr = 8 pts
// Derived classes can override this virtual function to change this.
// Return zero to disallow changing size.
//
int CFontUI::GrowFontSize(int ptSize, int dir)
{
	int incr = ptSize <= 12 ? 1 : ptSize < 32 ? 2 : ptSize < 48 ? 4 : 8;
	ptSize += dir>0 ? incr : -incr;;
	return ptSize;
}

//////////////////
// Create font from info in application profile. Reads info in the form
// facename,ptsize,weight,italic
//
BOOL CFontUI::GetProfileFont(LPCTSTR lpszKey, LPCTSTR lpszVal, CFont& font,
	CDC* pDC)
{
	CWinApp *pApp = AfxGetApp();
	ASSERT_VALID(pApp);
	CString s = pApp->GetProfileString(lpszKey, lpszVal);
	if (s.IsEmpty())
		return FALSE;

	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));
	lf.lfCharSet = DEFAULT_CHARSET;
	int bItalic;
	int iPtSize;

	// scanf is overkill, but I'm lazy
	if (_stscanf((LPCTSTR)s, _T("%[a-zA-Z ],%d,%d,%d"),
		lf.lfFaceName, &iPtSize, &lf.lfWeight, &bItalic) != 4)
		return FALSE;
	lf.lfHeight = MulDiv(-iPtSize, 	// convert ptsize to logical units
		::GetDeviceCaps(pDC ? pDC->m_hDC : ::GetDC(NULL), LOGPIXELSY), 72);
	lf.lfItalic = bItalic;	// because lf.lfItalic is a BYTE
	font.DeleteObject();		// bye 
	return font.CreateFontIndirect(&lf);
}

//////////////////
// Write font to app profile in the form "facename,ptsize,weight,italic"
//
BOOL CFontUI::WriteProfileFont(LPCTSTR lpszKey, LPCTSTR lpszVal, CFont& font,
	CDC* pDC)
{
	CWinApp *pApp = AfxGetApp();
	ASSERT_VALID(pApp);
	LOGFONT lf;
	font.GetLogFont(&lf);
	int iPtSize = MulDiv(-lf.lfHeight, 72,
		::GetDeviceCaps(pDC ? pDC->m_hDC : ::GetDC(NULL), LOGPIXELSY));
	CString s;
	s.Format(_T("%s,%d,%d,%d"), lf.lfFaceName, iPtSize, lf.lfWeight, lf.lfItalic);
	return pApp->WriteProfileString(lpszKey, lpszVal, s);
}

