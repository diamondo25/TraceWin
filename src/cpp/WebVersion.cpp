////////////////////////////////////////////////////////////////
// MSDN Magazine -- April 2003
// If this code works, it was written by Paul DiLascia.
// If not, I don't know who wrote it.
// Compiles with Visual Studio .NET on Windows XP. Tab size=3.
//
#include "stdafx.h"
#include "WebVersion.h"
#include "InetHandle.h"

//////////////////
// Check if connected to Internet.
//
BOOL CWebVersion::Online()
{
    DWORD dwState = 0; 
    DWORD dwSize = sizeof(DWORD);
	 return InternetQueryOption(NULL,
		 INTERNET_OPTION_CONNECTED_STATE, &dwState, &dwSize)
		 && (dwState & INTERNET_STATE_CONNECTED);
}

//////////////////
// Read version number as string into buffer
//
BOOL CWebVersion::ReadVersion(LPCTSTR lpFileName)
{
	ASSERT(lpFileName);
	CInternetHandle hInternet;
	CInternetHandle hFtpSession;
	CInternetHandle hFtpFile;

	m_version[0] = 0;
	m_dwError=0;								 // assume success
	m_errInfo[0]=0;							 // ..

	DWORD nRead=0;
	hInternet = InternetOpen(NULL, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if (hInternet!=NULL) {
		hFtpSession = InternetConnect(hInternet, m_lpServer,
			INTERNET_DEFAULT_FTP_PORT, NULL, NULL, INTERNET_SERVICE_FTP, 0, NULL);

		if (hFtpSession!=NULL) {
			hFtpFile = FtpOpenFile(hFtpSession, lpFileName,
				GENERIC_READ, FTP_TRANSFER_TYPE_ASCII, NULL);

			if (hFtpFile!=NULL) {
				InternetReadFile(hFtpFile, m_version, BUFSIZE, &nRead);
				if (nRead>0) {
					m_version[nRead] = 0;
					int Mhi,Mlo,mhi,mlo;
					sscanf(m_version, "%x,%x,%x,%x", &Mhi, &Mlo, &mhi, &mlo);
					dwVersionMS = MAKELONG(Mlo,Mhi);
					dwVersionLS = MAKELONG(mlo,mhi);
					return TRUE;
				}
			}
		}
	}

	// Failed: save error code and extended error info if any.
	m_dwError = GetLastError();
	if (m_dwError==ERROR_INTERNET_EXTENDED_ERROR) {
		DWORD dwErr;
		DWORD len = sizeof(m_errInfo)/sizeof(m_errInfo[0]);
		InternetGetLastResponseInfo(&dwErr, m_errInfo, &len);
	}
	return FALSE;
}
