// $Revision: 1.12 $
/* Copyright (c) 2005 - Thomas Hansen
 * Part of the SmartWin++ library example code
 * Permission to use under the same terms as the actual library
 * The license to the library can be found at http://smartwinlib.org
 * License for library is Open Source and BSD
 */
#include "SmartWin.h"
using namespace SmartWin;

// Demonstrates usage of the three Widgets that can be used without bringing in the complete SmartWin
// Note, this project does NOT link against any SmartWinxx.lib files!
// Neither does it "take over" the main function like a normal SmartWin++ application would do!!

int WINAPI WinMain( HINSTANCE hInstance,
			 HINSTANCE hPrevInstance,
			 LPSTR lpCmdLine,
			 int nCmdShow )
{
	WidgetLoadFileFree dlg;
	SmartUtil::tstring path = dlg.showDialog();
	WidgetMessageBoxFree msg;
	msg.show( path );
	WidgetSaveFileFree dlg2;
	path = dlg2.showDialog();
	msg.show( path );
	return EXIT_SUCCESS;
}
