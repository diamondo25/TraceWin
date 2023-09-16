all: TraceWinListener.dll TestTraceWin.exe

TraceWinListener.dll: TraceWinListener.cs
	csc /t:library $*.cs

TestTraceWin.exe: TestTraceWin.cs
	csc $*.cs /r:TraceWinListener.dll /d:TRACE

clean:
	del *.exe *.dll
