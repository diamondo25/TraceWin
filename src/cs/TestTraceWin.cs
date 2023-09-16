////////////////////////////////////////////////////////////////
// MSDN Magazine -- October 2002
// If this code works, it was written by Paul DiLascia.
// If not, I don't know who wrote it.
// Compiles with Visual Studio .NET on Windows XP.
//
using System;
using System.Reflection;
using System.Diagnostics;
using System.Windows.Forms;
using TraceWin;

// Note that this must be compiled with /d:TRACE
class MainApp {
   public static void Main(String[] args) {
		// Remove default listener if you want
//		Debug.Listeners.Remove("Default");
		Debug.Listeners.Add(new TraceWinListener());
		TRACE.WL("Hello, world");
		TRACE.WL("Today is {0}",DateTime.Now);
		TRACE.WL("Ciao!");
	}
};

public class TRACE
{
	public static void W(String fmt, params Object[] args)
	{
		Trace.Write(String.Format(fmt, args));
	}
	public static void WL(String fmt, params Object[] args)
	{
		Trace.WriteLine(String.Format(fmt, args));
	}
}


