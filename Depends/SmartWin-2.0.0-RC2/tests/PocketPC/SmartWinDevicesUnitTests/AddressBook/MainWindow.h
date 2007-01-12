// $Revision: 1.4 $
#ifndef MainWindow_h
#define MainWindow_h
#include "SmartWin.h"

class ABCentralWidget;

// The inheritence string is a bit more "difficult" since SmartWin++ is a 100% template based
// GUI Framework and Qt is not!
class ABMainWindow : public SmartWin::WidgetFactory< SmartWin::WidgetWindow, ABMainWindow >
{
public:
	ABMainWindow();

private:
	void setupMenuBar();
	void setupFileTools();
	void setupStatusBar();
	void setupCentralWidget();

	// Event handlers!
	void menuEventHandler( WidgetMenuPtr menu, unsigned item );
	void sized( const SmartWin::WidgetSizedEventResult & sz );

	// Widgets
	WidgetMenuPtr mainMenu;
	ABCentralWidget * central;

	// Data Members
	SmartUtil::tstring filename;
};

#endif
