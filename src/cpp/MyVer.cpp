////////////////////////////////////////////////////////////////
// TRACEWIN Copyright 1997-2003 Paul DiLascia
// If this code works, it was written by Paul DiLascia.
// If not, I don't know who wrote it.
//

#include "StdAfx.h"
#include "MyVer.h"

CMyVersionInfo::CMyVersionInfo()
{
	// get my own module (EXE) filename 
	TCHAR fn[_MAX_PATH];
	GetModuleFileName(NULL, fn, _MAX_PATH);

	// Get size of file version info
	DWORD dwDummyHandle; // will always be set to zero
	DWORD len = GetFileVersionInfoSize(fn, &dwDummyHandle);
	ASSERT(len>0);

	// read file version info
	BYTE* pVersionInfo = new BYTE[len]; // allocate version info
	VERIFY(::GetFileVersionInfo(fn, 0, len, pVersionInfo));

	// get value of root block (\) = VS_FIXEDFILEINFO
	LPVOID lpvi; // ptr returned
	UINT iLen;	 // length returned
	VERIFY(VerQueryValue(pVersionInfo, _T("\\"), &lpvi, &iLen));

	// copy fixed info to myself, which am derived from VS_FIXEDFILEINFO 
	*(VS_FIXEDFILEINFO*)this = *(VS_FIXEDFILEINFO*)lpvi;

	delete[] pVersionInfo;
}
