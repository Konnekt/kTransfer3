@ECHO OFF

if defined SMARTWINok_a (
	goto PLATFORMSDK
)

:: Visual C++ 7.1
if defined VS71COMNTOOLS  (
	if exist "%VS71COMNTOOLS%\vsvars32.bat" (
		echo -
		echo - Visual C++ 7.1 found.
		echo -
		call "%VS71COMNTOOLS%\vsvars32.bat"
		set SMARTWINok_a=true
		goto PLATFORMSDK
	)
)

:: Visual C++ 8.0
if defined VS80COMNTOOLS (
	if exist "%VS80COMNTOOLS%\vsvars32.bat" (
		echo -
		echo - Visual C++ 8.0 found.
		echo -
		call "%VS80COMNTOOLS%\vsvars32.bat"
		set SMARTWINok_a=true
		goto PLATFORMSDK
	)
)

:: Toolkit 2003
if defined VCToolkitInstallDir (
	if exist "%VCToolkitInstallDir%\vcvars32.bat" (
		echo -
		echo - VC 7.1 Toolkit found.
		echo -
		call "%VCToolkitInstallDir%\vcvars32.bat"
		set SMARTWINok_a=true
		goto PLATFORMSDK
	)
)


echo -
echo - No Visual C++ found, please set the enviroment variable 
echo - 
echo - VCToolkitInstallDir  or  VS71COMNTOOLS or VS80COMNTOOLS 
echo - 
echo - to your Visual Studio folder which contains vsvars32.bat.
echo - 
echo - Or call the vsvars32.bat.
echo -

goto ERROR


:PLATFORMSDK
if defined SMARTWINok_b (
	goto STARTCOMPILING
)

echo -
echo - 
echo - 

:: PLATFORM SDK SetEnv.bat:

:: already found
if defined MSSdk (
	set SMARTWINok_b=true
	goto STARTCOMPILING
)
if exist "%ProgramFiles%\Microsoft SDK\SetEnv.Bat" (
	call "%ProgramFiles%\Microsoft SDK\SetEnv.Bat"
	set SMARTWINok_b=true
	goto STARTCOMPILING
)
:: This is the normal path that the VC++ 7.1 installs the SDK in, we resort to this one as the LAST option!
if exist "%ProgramFiles%\Microsoft Visual Studio .NET 2003\SDK\v1.1\Bin\sdkvars.bat" (
	call "%ProgramFiles%\Microsoft Visual Studio .NET 2003\SDK\v1.1\Bin\sdkvars.bat"
	set SMARTWINok_b=true
	goto STARTCOMPILING
)
if exist "%ProgramFiles%\Microsoft Platform SDK\SetEnv.Cmd" (
	call "%ProgramFiles%\Microsoft Platform SDK\SetEnv.Cmd"
	set SMARTWINok_b=true
	goto STARTCOMPILING
)
:: this processes the value from the installer
set SMARTWIN_TMP=%1
if defined SMARTWIN_TMP (
	if exist %*\SetEnv.Bat (
		call %*\SetEnv.Bat
		set SMARTWINok_b=true
		goto STARTCOMPILING
	)
)
set SMARTWIN_TMP=

echo -
echo - No SDK found.
echo -
echo -
goto ERROR
		


:STARTCOMPILING


:: SmartWin buid process

if defined SMARTWINok_a (
if defined SMARTWINok_b (

@ECHO ON

cd source
call make.msvc.bat
cd ..

cd SmartSOAP
call make.msvc.bat
cd ..

cd tests
call make.msvc.bat
cd ..

goto LEAVE

)
)

:ERROR
echo -
echo -
echo - An error occured. Compiling aborted.
echo - 
pause



:LEAVE
