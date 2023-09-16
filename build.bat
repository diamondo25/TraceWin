@echo off
: This batch file stuff for the web: pxsamp.zip
:
setlocal

set HOMEPAGE=c:\apd\web
call clean
cd src\cpp
nmake rel
cp release\tracewin.exe ...
call clean
cd ..\cs
nmake
cp TraceWinListener.dll ...
call clean
cd ...
echo packdir TraceWin

