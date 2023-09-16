////////////////////////////////////////////////////////////////
// TraceWinListener Copyright 2003 Paul DiLascia
// If this code works, it was written by Paul DiLascia.
// If not, I don't know who wrote it. 
// Compiles with VC7 on Windows XP. Tab size = 3.
//
using System;
using System.Runtime.InteropServices;
using System.Diagnostics;
using System.Reflection;
using System.Runtime.CompilerServices;

[assembly: AssemblyTitle("TraceWinListener")]
[assembly: AssemblyDescription(".NET listener for TraceWin applet")]
[assembly: AssemblyConfiguration("")]
[assembly: AssemblyCompany("Paul DiLascia")]
[assembly: AssemblyProduct("TraceWinListener")]
[assembly: AssemblyCopyright("Copyright 2002 Paul DiLascia")]
[assembly: AssemblyVersion("1.0.0.1")]
[assembly: AssemblyDelaySign(false)]
[assembly: AssemblyKeyFile("")]
[assembly: AssemblyKeyName("")]

//////////////////
//	This module implements a TraceWin listener for .NET.
// Use it to see trace diagnostics in my TraceWin applet window.
// To install in your app, add the following lines:
//
//		using TraceWin;
//		Debug.Listeners.Add(new TraceWinListener());
//
// You must add a reference to TraceWinListener.dll in your project and
// don't forget to compile with /d:TRACE.
//
namespace TraceWin {

//////////////////
// Handy trace listener writes to TraceWin. Neat!
//
public class TraceWinListener : TraceListener {

	[StructLayout(LayoutKind.Sequential)]
	private struct COPYDATASTRUCT {
		public Int32 dwData;
		public Int32 cbData;
		public IntPtr lpData;
	}

	[DllImport("user32.dll")]
	private static extern long SendMessage(Int32 hwnd, Int32 msg,
		Int32 hwndFrom, ref COPYDATASTRUCT cd);

	[DllImport("user32.dll")]
	private static extern Int32 FindWindow(String classname, String text);

	[DllImport("kernel32.dll")]
	private static extern Int32 GlobalSize(IntPtr hmem);

	public override void Write(String s) {
		// Don't forget to use IndentLevel and IndentSize!
		if (Trace.IndentLevel>0) {
			s = s.Insert(0,new String(' ',
				Trace.IndentSize * Trace.IndentLevel));
		}

		// Send WM_COPYDATA message to trace window
		Int32 hTraceWnd = FindWindow("MfxTraceWindow",null);
		if (hTraceWnd!=0) {
			COPYDATASTRUCT cd = new COPYDATASTRUCT();
			Int32 WM_COPYDATA = 0x004A;	 // Win32 API message id
			cd.dwData = 0x6e697775;			 // magic message code TraceWin
			cd.lpData = Marshal.StringToHGlobalUni(s);
			cd.cbData = GlobalSize(cd.lpData);
			SendMessage(hTraceWnd, WM_COPYDATA, 0, ref cd);
			Marshal.FreeHGlobal(cd.lpData);
		}
	}

	public override void WriteLine(String s) {
		Write(s+"\n");
	}
}

} // namespace

