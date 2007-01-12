// $Revision: 1.3 $
/* Copyright (c) 2005 - Thomas Hansen
 * Part of the SmartWin++ library example code
 * Permission to use under the same terms as the actual library
 * The license to the library can be found at http://smartwinlib.org
 * License for library is Open Source and BSD
 */
#include "SmartWin.h"
using namespace SmartWin;
#include "MdiFrame.h"
using namespace std;

/*  A sample SmartWin++ program.

See http://smartwin.sourceforge.net for documentation.

Briefly, its a C++ template base wrapper around the WIN32 api
that allows GUI operations.

HOWTO create a new SmartWin++ project:
1)  File | New | Project | Visual C++ Projects | Win32 | Win32 project

2)  Project | Properties | Linker | Input | Additional Dependencies =
   comctl32.lib crypt32.lib wininet.lib smartwind.lib

3)  Project | Properties | C++ | Language | Enable run-time type info = YES

which prevents this error:
Command.h(72) : warning C4541:
'typeid' used on polymorphic type 'SmartWin::Command' with /GR-;
unpredictable behavior may result

*/

int SmartWinMain( Application & app )
{
	MdiFrame * aw3 = new MdiFrame();
	aw3->init();
	return ( app.run() );
}
