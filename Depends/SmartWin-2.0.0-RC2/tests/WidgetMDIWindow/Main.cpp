// $Revision: 1.2 $

#include <sstream>
#include <fstream>
#include "SmartWin.h"
#include "MDIChild.h"
#include "WidgetMain.h"

using namespace SmartWin;

// Unit test for WidgetTextBox, tries to instantiate all different forms of WidgetTextBox and also tries to use all functions
int SmartWinMain( Application & app )
{
	// Widget (Factory) version
	WidgetMain * testWnd1 = new WidgetMain;
	testWnd1->initAndCreate();
	return app.run();
}
