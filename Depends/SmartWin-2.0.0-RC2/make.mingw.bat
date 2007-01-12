@ECHO OFF
set SMARTWIN_TMP=%1
if defined SMARTWIN_TMP (
	set PATH=%~1;%PATH%
)
set SMARTWIN_TMP=
@ECHO ON

mingw32-make -f Makefile.mingw check

@ECHO OFF
REM cd SmartSOAP\Tester1
REM mingw32-make -f Makefile.mingw 
REM cd ..\..
