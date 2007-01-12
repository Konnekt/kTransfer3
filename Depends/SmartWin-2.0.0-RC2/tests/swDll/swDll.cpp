// $Revision: 1.4 $
/* swDll.cpp : Defines the entry point for the DLL module.
   Its API definition is swdll.h

This DLL is tested by the swDLLMain project (swDLLMain.cpp)
Both projects are in the swDLL directory.

An example that SmartWin++ can be used in a DLL. The calling program does not link with SW.
It works with the SmartWin++ window in helloDLL.h because we call the static function
	Application::neededSmartWinInit(); during the DLL startup.

*/
#include "stdafx.h"

#define DLLMODE dllexport
#include "swdll.h"
#include "helloDll.h"

// Called
BOOL APIENTRY DllMain( HANDLE hModule,
					   DWORD ul_reason_for_call,
					   LPVOID lpReserved
					 )
{
	if ( DLL_PROCESS_ATTACH == ul_reason_for_call )
	{
		Application::neededSmartWinInit(); // Prepare for SmartWin++
	}

	if ( DLL_PROCESS_DETACH == ul_reason_for_call )
	{
		bool corruptMemMemLeak;
		try
		{
			Application::checkCorruptOrMemleak( corruptMemMemLeak );
		} catch ( xCeption & err )
		{
			Application::reportErr( err, corruptMemMemLeak );
		}
	}

	return TRUE;
}

// This just tests the DLL function call process
__declspec( dllexport )
bool getNumberAndString( int * numb, char str[] )
{
	* numb = 23;
	SmartUtil::tstring tstr = "hello world from a DLL";
	strcpy( str, tstr.c_str() );

	return true;
}

// The DLL function that invokes a SmartWin++ window.
// The two variables are set upon the close of the window, in HelloDllClass.
//
__declspec( dllexport )
bool guiGetNumberAndString( int * numb, char str[] )
{
	HelloDllClass * testHello = new HelloDllClass;
	testHello->initAndCreate( numb, str );
	Application::instance().run(); // Stops when window closes.
	return true;
}

// Just so "SmartWinMain" is not undefined when we link the DLL.
// Since WinMain is not called in a DLL, this function is never called.
int SmartWinMain( Application & app )
{
	return 0;
}
