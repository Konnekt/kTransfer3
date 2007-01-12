// $Revision: 1.8 $
// Unit test for WidgetSpinner, tries to instantiate all different forms of
// WidgetSpinner and also tries to use all functions

#include "SmartWin.h"
#include <sstream>
using namespace SmartWin;

// The "factory" version
class WidgetTest1
	: public WidgetFactory< WidgetWindow, WidgetTest1 >
{
	WidgetStatusBarPtr status;
public:

	WidgetTest1()
	{}

	void scrolling( WidgetSpinnerPtr spinner )
	{
		status->setText( boost::lexical_cast< SmartUtil::tstring >( spinner->getValue() ) );
	}

	void resized( const WidgetSizedEventResult & sz )
	{
		status->refresh();
	}

	void initAndCreate()
	{
		WidgetWindow::Seed windowCS;

		windowCS.caption = _T( "WidgetSpinner compile and execute test" );
		windowCS.background = CreateSolidBrush( RGB( 0, 0, 125 ) );
		windowCS.location = SmartWin::Rectangle( 100, 100, 170, 220 );
		createWindow( windowCS );
		status = createStatusBar();

		onSized( & WidgetTest1::resized );

		WidgetSpinner::Seed spinnerCS;

		spinnerCS.location = SmartWin::Rectangle( 30, 50, 50, 100 );
		spinnerCS.minValue = - 5;
		spinnerCS.maxValue = 5;
		WidgetSpinnerPtr spinner = createSpinner( spinnerCS );
		// Default a spinner is created with vertically scrolling
		spinner->onScrollVert( & WidgetTest1::scrolling );

		spinnerCS.style = UDS_SETBUDDYINT | UDS_WRAP | UDS_HORZ | WS_VISIBLE;
		spinnerCS.minValue = - 10;
		spinnerCS.maxValue = 10;
		spinnerCS.location = SmartWin::Rectangle( 70, 50, 50, 100 );
		spinner = createSpinner( spinnerCS );
		// Since this spinner is created laying horizontally then we will have to
		// trap the Horizontally scroll event instead of the vertical one...
		spinner->onScrollHorz( & WidgetTest1::scrolling );

		// Buddy control...
		WidgetTextBoxPtr box = createTextBox();
		box->setBounds( 30, 10, 90, 20 );
		box->setText( _T( "0" ) );
		spinner->assignBuddy( box );
	}
};

int SmartWinMain( Application & app )
{
	// Widget (Factory) version
	WidgetTest1 * testWnd1 = new WidgetTest1;
	testWnd1->initAndCreate();
	return app.run();
}
