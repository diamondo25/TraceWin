////////////////////////////////////////////////////////////////
// PixieLib(TM) Copyright 1997-1999 Paul DiLascia
// If this code works, it was written by Paul DiLascia.
// If not, I don't know who wrote it.
//
#include "StdAfx.h"
#include "WinPlace.h"

const TCHAR FormatWP[] = _T("%d,%d,%d,%d,%d,%d,%d,%d,%d,%d");
const int NFIELDS = 10;

CWindowPlacement::CWindowPlacement()
{
	// Note: "length" is inherited from WINDOWPLACEMENT
	length = sizeof(WINDOWPLACEMENT);
}

CWindowPlacement::~CWindowPlacement()
{
}

//////////////////
// Restore window placement from profile key
//
BOOL CWindowPlacement::Restore(LPCTSTR lpszKey, LPCTSTR lpszVal, CWnd* pWnd)
{
	if (!GetProfileWP(lpszKey, lpszVal))
		return FALSE;

	// Only restore if window intersets the screen.
	//
	CRect rcTemp, rcScreen(0,0,GetSystemMetrics(SM_CXSCREEN),
		GetSystemMetrics(SM_CYSCREEN));
	if (!::IntersectRect(&rcTemp, &rcNormalPosition, &rcScreen))
		return FALSE;

	pWnd->SetWindowPlacement(this);	// set placement
	return TRUE;
}

//////////////////
// Get window placement from profile.
//
BOOL CWindowPlacement::GetProfileWP(LPCTSTR lpszKey, LPCTSTR lpszVal)
{
	CWinApp *pApp = AfxGetApp();
	ASSERT_VALID(pApp);
	CString s = pApp->GetProfileString(lpszKey, lpszVal);
	if (s.IsEmpty())
		return FALSE;

	// scanf is overkill, but I'm lazy
	return _stscanf((LPCTSTR)s, FormatWP,
		&showCmd,
		&flags,
		&ptMinPosition.x,
		&ptMinPosition.y,
		&ptMaxPosition.x,
		&ptMaxPosition.y,
		&rcNormalPosition.left,
		&rcNormalPosition.right,
		&rcNormalPosition.top,
		&rcNormalPosition.bottom) == NFIELDS;
}

////////////////
// Save window placement in app profile
//
void CWindowPlacement::Save(LPCTSTR lpszKey, LPCTSTR lpszVal, CWnd* pWnd)
{
	pWnd->GetWindowPlacement(this);
	WriteProfileWP(lpszKey, lpszVal);
}

//////////////////
// Write window placement to app profile
//
BOOL CWindowPlacement::WriteProfileWP(LPCTSTR lpszKey, LPCTSTR lpszVal)
{
	CWinApp *pApp = AfxGetApp();
	ASSERT_VALID(pApp);
	CString s;
	s.Format(FormatWP,
		showCmd,
		flags,
		ptMinPosition.x,
		ptMinPosition.y,
		ptMaxPosition.x,
		ptMaxPosition.y,
		rcNormalPosition.left,
		rcNormalPosition.right,
		rcNormalPosition.top,
		rcNormalPosition.bottom);
	return pApp->WriteProfileString(lpszKey, lpszVal, s);
}

// The ugly casts are required to help the VC++ 3.0 compiler decide which
// operator<< or operator>> to use. If you're using VC++ 4.0 or later, you 
// can delete this stuff.
//
#if (_MSC_VER < 0x400) // VC++ 4.0
#define UINT_CAST (LONG)
#define UINT_CASTREF (LONG&)
#else
#define UINT_CAST
#define UINT_CASTREF
#endif

//////////////////
// Write window placement to archive
// WARNING: archiving functions are untested.
//
CArchive& operator<<(CArchive& ar, const CWindowPlacement& wp)
{
	ar << UINT_CAST wp.length;
   ar << UINT_CAST wp.flags;
	ar << UINT_CAST wp.showCmd;
   ar << wp.ptMinPosition;
   ar << wp.ptMaxPosition;
   ar << wp.rcNormalPosition;
	return ar;
}

//////////////////
// Read window placement from archive
// WARNING: archiving functions are untested.
//
CArchive& operator>>(CArchive& ar, CWindowPlacement& wp)
{
	ar >> UINT_CASTREF wp.length;
   ar >> UINT_CASTREF wp.flags;
	ar >> UINT_CASTREF wp.showCmd;
   ar >> wp.ptMinPosition;
   ar >> wp.ptMaxPosition;
   ar >> wp.rcNormalPosition;
	return ar;
}
