////////////////////////////////////////////////////////////////
// TRACEWIN Copyright 1997-2003 Paul DiLascia
// If this code works, it was written by Paul DiLascia.
// If not, I don't know who wrote it.
//

// tell linker to link with version.lib for VerQueryValue, etc.
#pragma comment(linker, "/defaultlib:version.lib")

///////////////////
// Handy class to retrieve my own version info from resource.
//
class CMyVersionInfo : public VS_FIXEDFILEINFO
{
public:
	CMyVersionInfo();
};

