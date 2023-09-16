////////////////////////////////////////////////////////////////
// TraceWin Copyright 1995-2003 Paul DiLascia
// If this program works, it was written by Paul DiLascia.
// If not, I don't know who wrote it.
//
// ***************************************************************************
// TraceWin is a tool that displays diagnostic (TRACE, ATLTRACE) output
// in the TraceWin applet window.
//
// To use TraceWin, you must #include this file somewhere in your main program
// file, but only once--i.e. NOT in StdAfx.h--or you'll get multiply-defined
// symbol errors in the linker. This file contains an auto-initializing
// singleton instance, so all you have to do is #include the file. 
//
// To see the output, you also need TraceWin.exe, which if you don't
// have already, you can download from www.dilascia.com/TraceWin.htm
//
// ***************************************************************************
//
#ifdef _DEBUG

// Window class name used by the main window of the TRACEWIN applet.
#define TRACEWND_CLASSNAME _T("MfxTraceWindow") // backwards compat

// ID sent as COPYDATASRUCT::dwData to identify the WM_COPYDATA message
// as coming from an app using TraceWin.
#define ID_COPYDATA_TRACEMSG MAKELONG(MAKEWORD('t','w'),MAKEWORD('i','n'))

//////////////////
// Self-initializing CRT-based trace "listener" hooks
// itself into C runtime debug system.
//
class CTraceWinListener {
protected:
	static CTraceWinListener singleton;	 // one-and-only instance
	static _CRT_REPORT_HOOK pfnOldHook;	 // ptr to old reporting hook
	
	// ctor installs hook
	CTraceWinListener()
	{
		// if this bombs, you probably #included TraceWin.h twice
		ASSERT(pfnOldHook == NULL);
		pfnOldHook = _CrtSetReportHook(MyCrtReportHook);
	}

	// dtor removes hook
	~CTraceWinListener()
	{
		pfnOldHook = _CrtSetReportHook(pfnOldHook);
	}

	// hook function installed in CRT: passes text to TraceWin
	static int __cdecl MyCrtReportHook(int nRptType, char *szMsg, int* pRes)
	{
		// call the old report hook if there was one
		if (pfnOldHook && (*pfnOldHook)(nRptType, szMsg, pRes)) {
			return TRUE;
		}

		// don't hook asserts
		if (nRptType == _CRT_ASSERT)
			return FALSE;

		// Find trace window. Note that it's not really safe to store
		// this HWND if I want to allow closing/restarting TraceWin. The
		// window handle could change!
		HWND hTraceWnd = ::FindWindow(TRACEWND_CLASSNAME, NULL);
		if (hTraceWnd) {
			COPYDATASTRUCT cds;
			cds.dwData = ID_COPYDATA_TRACEMSG;
			cds.cbData = (DWORD)strlen(szMsg);
			cds.lpData = (void*)szMsg;
			WPARAM wParam = 0;
			::SendMessage(hTraceWnd, WM_COPYDATA, wParam, (LPARAM)&cds);
			*pRes = FALSE;
		}
		return FALSE; // don't stop next hook
	}
};

// one-and-only singleton instance automatically installs debug hook
CTraceWinListener CTraceWinListener::singleton;
_CRT_REPORT_HOOK  CTraceWinListener::pfnOldHook=NULL;

#endif // _DEBUG
