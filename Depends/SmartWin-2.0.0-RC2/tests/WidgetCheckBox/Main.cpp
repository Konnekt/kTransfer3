// $Revision: 1.11 $
/*!
  \file Main.cpp
  \brief WidgetCheckBox test

  Unit test for WidgetTextBox, tries to instantiate all different forms of
  WidgetTextBox and also tries to use all functions
*/
#include "SmartWin.h"
#include <vector>
#include <assert.h>
using namespace SmartWin;

// The "factory" version
class WidgetTest1
	: public WidgetFactory< WidgetWindow, WidgetTest1 >
{
public:
	WidgetTest1()
	{}

	void clicked( WidgetCheckBoxPtr chk )
	{
		SmartUtil::tstring checked = chk->getChecked() ? _T( "CHECKED" ) : _T( "NOT CHECKED" );
		createMessageBox().show( chk->getText() + _T( " " ) + checked );
	}

	void initAndCreate()
	{
		WidgetWindow::Seed windowCS;

		// Uncomment the following code to get the blue background shown in the
		// screenshoots. Currently, only the background of the main window can be
		// changed using Seed.
		
		//windowCS.background = CreateSolidBrush( RGB( 0, 0, 125 ) );
		windowCS.location = SmartWin::Rectangle( 100, 100, 400, 220 );
		windowCS.caption = _T( "WidgetCheckBox compile and execute test" );
		createWindow( windowCS );

		WidgetCheckBox::Seed checkBoxCS;

		checkBoxCS.location = SmartWin::Rectangle( 100, 50, 150, 50 );
		checkBoxCS.caption = _T( "Push Me" );
		checkBoxCS.font = createFont( _T( "Times New Roman" ), 0, 10, 1, ANSI_CHARSET, true, false, false, 0, OUT_DEFAULT_PRECIS );
		WidgetCheckBoxPtr chk = createCheckBox( checkBoxCS );
		assert( chk->getText() == _T( "Push Me" ) );
		chk->onClicked( & WidgetTest1::clicked );
	}
};

void clicked( WidgetTest1 * parent, WidgetTest1::WidgetCheckBoxPtr chk )
{
	WidgetTest1::WidgetMessageBox box = parent->createMessageBox();
	box.show( chk->getText() );
	chk->setEnabled( false );
}

int SmartWinMain( Application & app )
{
	// Widget (Factory) version
	WidgetTest1 * testWnd1 = new WidgetTest1;
	testWnd1->initAndCreate();

	// Creating "free" button...
	WidgetTest1::WidgetCheckBox::Seed checkBoxCS;

	checkBoxCS.location = SmartWin::Rectangle( 100, 110, 120, 50 );
	checkBoxCS.caption = _T( "I was created free" );
	WidgetTest1::WidgetCheckBoxPtr chk = testWnd1->createCheckBox( checkBoxCS );
	chk->onClicked( clicked );

	return app.run();
}
