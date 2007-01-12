// $Revision: 1.9 $

#include "SmartWin.h"
using namespace SmartWin;

// Inherited WidgetButton
class MyButton
	: public WidgetButton< MyButton, SmartWin::MessageMapPolicyNormalWidget >
{
public:
	MyButton( Widget * parent )
		// Note!
		// We MUST explicitly call Widget's Constructor since it's a diamond inheritence, at least until bjarne's C++0x gives us Constructor Forwarding... ;)
		: Widget( parent ),
		WidgetButton< MyButton, SmartWin::MessageMapPolicyNormalWidget >( parent )
	{
	}

	// Event handler for "click" event, maark the "funny" paramater...
	void click( WidgetButton< MyButton, SmartWin::MessageMapPolicyNormalWidget > * btn )
	{
		WidgetMessageBox< SmartWin::Widget > dlg( btn->getParent() );
		dlg.show( btn->getText() );
	}

	void init()
	{
		// Doing "init" stuff
		create();
		setBounds( 100, 200, 200, 200 );
		setText( _T( "Custom inherited button" ) );
		onClicked( & MyButton::click );
	}
};

class WidgetTest
	: public WidgetFactory< WidgetWindow, WidgetTest >
{
public:

	// Note! Static event handler (takes pointer to parent)
	static void butClicked2( WidgetTest * wnd, WidgetButtonPtr btn )
	{
		wnd->createMessageBox().show( btn->getText() + _T( ": 2nd" ) );

		// Note!
		// Here we are IN RUNTIME SWITCHING event handler
		btn->onClicked( & WidgetTest::butClicked );
	}

	// ** Note ** we get the button that was clicked as param
	// This means we can "reuse" event handlers and still know who triggered the event...
	void butClicked( WidgetButtonPtr btn )
	{
		createMessageBox().show( btn->getText() + _T( ": 1st" ) );

		// Note!
		// Here we are IN RUNTIME SWITCHING event handler
		btn->onClicked( & WidgetTest::butClicked2 );
	}

	void init()
	{
		WidgetWindow::Seed windowCS;

		windowCS.location = SmartWin::Rectangle( 100, 100, 600, 600 );
		windowCS.background = CreateSolidBrush( RGB( 0, 0, 125 ) );
		windowCS.caption = _T( "WidgetButton Trigger Happy..." );
		createWindow( windowCS );

		// Creating a button
		WidgetButtonPtr btn = createButton();
		btn->setBounds( 100, 100, 200, 100 );
		btn->setText( _T( "Testing Button1" ) );
		btn->onClicked( & WidgetTest::butClicked );

		// Creating a SUBCLASSED button
		MyButton * btn2 = new MyButton( this );
		btn2->init();
	}
};

int SmartWinMain( Application & app )
{
	// Initializing main window
	WidgetTest * test = new WidgetTest();
	test->init();

	// Creating a button
	WidgetTest::WidgetButtonPtr btn = test->createButton();
	btn->setBounds( 300, 100, 200, 100 );
	btn->setText( _T( "Testing Button ** 2 **" ) );
	btn->onClicked( & WidgetTest::butClicked );

	// Running application
	return app.run();
}
