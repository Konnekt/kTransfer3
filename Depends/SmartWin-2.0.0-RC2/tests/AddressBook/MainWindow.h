// $Revision: 1.7 $
/* Copyright (c) 2005 - Thomas Hansen
 * Part of the SmartWin++ library example code
 * Permission to use under the same terms as the actual library
 * The license to the library can be found at http://smartwinlib.org
 * License for library is Open Source and BSD
 */
#ifndef MainWindow_h
#define MainWindow_h

#include "SmartWin.h"

class ABCentralWidget;

// The inheritence string is a bit more "difficult" since SmartWin++ is a 100% template based
// GUI Framework and Qt is not!
class ABMainWindow : public sw::WidgetFactory< sw::WidgetWindow, ABMainWindow >
{
public:
	ABMainWindow();

private:
	void setupMenuBar();
	void setupMenuItems( bool central_widget_alive );
	void setupFileTools();
	void setupStatusBar();
	void setupCentralWidget();

	// Event handlers!
	void menuEventHandler( WidgetMenuPtr menu, unsigned item );
	void sized( const sw::WidgetSizedEventResult & sz );

	// Widgets
	WidgetMenuPtr itsMainMenu;
	ABCentralWidget * itsCentral;

	// Data Members
	SmartUtil::tstring itsFilename;
};

#endif
