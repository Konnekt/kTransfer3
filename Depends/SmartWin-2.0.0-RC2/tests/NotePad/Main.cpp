// $Revision: 1.3 $
/* Copyright (c) 2005 - Thomas Hansen
 * Part of the SmartWin++ library example code
 * Permission to use under the same terms as the actual library
 * The license to the library can be found at http://smartwinlib.org
 * License for library is Open Source and BSD
 */
#include "SmartWin.h"
#include "MainWindow.h"

int SmartWinMain( SmartWin::Application & app )
{
	new MainWindow();
	return app.run();
}
