////////////////////////////////////////////////////////////////
// PixieLib(TM) Copyright 1997-1999 Paul DiLascia
// If this code works, it was written by Paul DiLascia.
// If not, I don't know who wrote it.
//
#pragma once

////////////////
// Manage user interface to changing a font. Handy for Edit | Font... command
// and Font Bigger/Font Smaller buttons. For example of how to use, see DIBVIEW
// sampple program.
//
class DLLCLASS CFontUI {
protected:
	int		m_nFontPtSizeMin;	// min..
	int		m_nFontPtSizeMax;	// ..and max font size in points

	// override this to change grow/shrink algorithm
	virtual int GrowFontSize(int ptSize, int dir);

public:
	CFontUI();
	virtual ~CFontUI();

	// universal function for bigger/smaller/dialog
	BOOL OnChangeFont(CFont& font, int op, CWnd* pWnd=NULL, DWORD dwFlags=0);

	void SetMinMaxFontSize(int nMin, int nMax) {
		m_nFontPtSizeMin = nMin;
		m_nFontPtSizeMax = nMax;
	}

	// handy conversion functions
	int  GetFontPointSize(CFont& font, CDC& dc);
	BOOL SetFontPointSize(CFont& font, CDC& dc, int pts);

	// Functions to save/restore font in profile (registry settings or .ini)
	BOOL WriteProfileFont(LPCTSTR lpszKey, LPCTSTR lpszVal, CFont& font,
		CDC* pDC=NULL);
	BOOL GetProfileFont(LPCTSTR lpszKey, LPCTSTR lpszVal, CFont& font,
		CDC* pDC=NULL);
};
