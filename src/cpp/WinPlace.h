////////////////////////////////////////////////////////////////
// PixieLib(TM) Copyright 1997-1999 Paul DiLascia
// If this code works, it was written by Paul DiLascia.
// If not, I don't know who wrote it.
//
#pragma once

////////////////
// CWindowPlacement reads and writes WINDOWPLACEMENT from/to application
// profile and CArchive. This lets you completely save and restore the
// position of a window across user sessions. For example how to use it,
// see the TRACEWIN sample program.
//
struct DLLCLASS CWindowPlacement : public WINDOWPLACEMENT {
	CWindowPlacement();
	~CWindowPlacement();
	
	// read/write to app profile
	BOOL GetProfileWP(LPCTSTR lpszKey, LPCTSTR lpszVal);
	BOOL WriteProfileWP(LPCTSTR lpszKey, LPCTSTR lpszVal);

	// save/restore window pos (from app profile)
	void Save(LPCTSTR lpszKey, LPCTSTR lpszVal, CWnd* pWnd);
	BOOL Restore(LPCTSTR lpszKey, LPCTSTR lpszVal, CWnd* pWnd);

	// save/restore from archive
	friend CArchive& operator<<(CArchive& ar, const CWindowPlacement& wp);
	friend CArchive& operator>>(CArchive& ar, CWindowPlacement& wp);
};
