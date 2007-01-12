// $Revision: 1.4 $
/* Copyright (c) 2005 - Thomas Hansen
 * Part of the SmartWin++ library example code
 * Permission to use under the same terms as the actual library
 * The license to the library can be found at http://smartwinlib.org
 * License for library is Open Source and BSD
 */

// TODO: I don't know why, but when using SmartSOAP together with SmartWin you need to include the files in different order according
// to if you build for Windows CE (Pocket PC, WindowsMobile etc) then if you build for Desktop Windows 32 systems...
#ifdef WINCE
#include "SmartWin.h"
#include "SmartSOAP.h"
#else
#ifdef __GNUC__
#include "SmartWin.h"
#include "SmartSOAP.h"
#else
#include "SmartSOAP.h"
#include "SmartWin.h"
#endif
#endif

#include "MainWindow.h"

int SmartWinMain( SmartWin::Application & app )
{
	MainWindow * tmp = new MainWindow();
	tmp->init();
	return app.run();
}
