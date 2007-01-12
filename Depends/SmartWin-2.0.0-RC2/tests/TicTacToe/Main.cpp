// $Revision: 1.2 $
#include "SmartWin.h"
#include "MainWindow.h"

// Unit test for WidgetTextBox, tries to instantiate all different forms of WidgetTextBox and also tries to use all functions
int SmartWinMain( sw::Application & app )
{
	MainWindow * ptr = new MainWindow();
	app.setHeartBeatFunction( ptr );
	ptr->init();
	return app.run();
}
