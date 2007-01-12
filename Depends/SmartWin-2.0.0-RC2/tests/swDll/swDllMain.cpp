// $Revision: 1.3 $
// swDllMain.cpp : Defines the entry point for the C++ application.
// This program uses swDll.dll whose source is swDll.cpp, and API definition is swdll.h
// Both projects are in the swDLL directory.
//
#define DLLMODE dllimport
#include "swdll.h"
#include <iostream>

int main( int argc, char * argv[] )
{
	int numb;
	char str[99];
	if ( getNumberAndString( & numb, ( char * ) & str ) )
	{
		std::cout << numb << " " << str << std::endl;
	}

	// Same thing but this time the DLL does it with a GUI.
	if ( guiGetNumberAndString( & numb, ( char * ) & str ) )
	{
		std::cout << numb << " " << str << std::endl;
	}

	return 0;
}
