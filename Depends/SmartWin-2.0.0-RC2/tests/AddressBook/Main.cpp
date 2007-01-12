// $Revision: 1.4 $
/* Copyright (c) 2005 - Thomas Hansen
 * Part of the SmartWin++ library example code
 * Permission to use under the same terms as the actual library
 * The license to the library can be found at http://smartwinlib.org
 * License for library is Open Source and BSD
 */
// Address Book sample from Qt, see: http://doc.trolltech.com/3.0/addressbook-example.html for the Qt version!
// Simple application to demonstrate the differences between SmartWin and Qt

// The Qt sample contains 604 lines of code with comments and contains 5 files and 2 classes
// The SmartWin++ sample contains 616 lines of code with comments and contains 7 files and 3 classes
// I think this is a just comparison since both code examples are clean and contains comments where applicable
// and uses their "best practices"...
#include "SmartWin.h"
#include "MainWindow.h"

// In Qt you must yourself construct your Application object, in SmartWin++ you get one for free!
// :)
// Also SmartWin++ uses NAMESPACES extensively so you don't have to worry for nameclashes!
int SmartWinMain( sw::Application & app )
{
	// In SmartWin++ you don't need to do anything with the returned object unless you want to
	// We COULD have done it the same way as the Qt sample though...
	new ABMainWindow();

	// In Qt you have the QApplication::exec function, in SmartWin it's called "run"
	return app.run();
}
