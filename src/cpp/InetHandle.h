////////////////////////////////////////////////////////////////
// MSDN Magazine -- April 2003
// If this code works, it was written by Paul DiLascia.
// If not, I don't know who wrote it.
// Compiles with Visual Studio .NET on Windows XP. Tab size=3.
//
#pragma once

//////////////////
// This handy class encapsulates an internet handle so you never have to
// remember to call CloseInternetHandle. More importantly,
// CInternetHandle lets you postpone the call to CloseInternetHandle
// until control leaves the scope of your function, so you can get error
// information when something bad happens before CloseInternetHandle
// clears the error code.
//
class CInternetHandle {
protected:
	HINTERNET m_handle; // underlying handle

public:
	CInternetHandle() : m_handle(NULL) { }
	CInternetHandle(HINTERNET h) : m_handle(h) { }

	~CInternetHandle() { Close(); }

	// Close handle and set to NULL so I don't close again.
	void Close() {
		if (m_handle) {
			InternetCloseHandle(m_handle);
			m_handle = NULL;
		}
	}

	// Assignment from HINTERNET
	CInternetHandle& operator= (HINTERNET h)
	{
		ASSERT(m_handle==NULL); // only initial assignment, not copy
		m_handle = h;
		return *this;
	}

	// cast to HINTERNET
	operator HINTERNET() {
		return m_handle;
	}
};

