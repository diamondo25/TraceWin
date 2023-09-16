////////////////////////////////////////////////////////////////
// MSDN Magazine -- April 2003
// If this code works, it was written by Paul DiLascia.
// If not, I don't know who wrote it.
// Compiles with Visual Studio .NET on Windows XP. Tab size=3.
//
#pragma once

//////////////////
// This class encapsulates over-the-web version checking. It expects a
// text version file that contains 4 number separated by commas, the
// same format for FILEVERSION and PRODUCTVERSION in VS_VERSION_INFO.
// ReadVersion reads these numbers into dwVersionMS and dwVersionLS.
//
class CWebVersion {
protected:
	enum { BUFSIZE = 64 };
	LPCTSTR m_lpServer;						 // server name
	DWORD	  m_dwError;						 // most recent error code
	TCHAR   m_errInfo[256];					 // extended error info
	char	  m_version[BUFSIZ];				 // version number as text

public:
	DWORD dwVersionMS;		// version number: most-sig 32 bits
	DWORD dwVersionLS;		// version number: least-sig 32 bits

	CWebVersion(LPCTSTR server) : m_lpServer(server), m_version{0}, m_errInfo{0}, m_dwError(0), dwVersionLS(0), dwVersionMS(0) { }

	static  BOOL Online();
	BOOL	  ReadVersion(LPCTSTR lpFileName);
	LPCSTR  GetVersionText()		 { return m_version; }
	DWORD   GetError()				 { return m_dwError; }
	LPCTSTR GetExtendedErrorInfo() { return m_errInfo; }
};

