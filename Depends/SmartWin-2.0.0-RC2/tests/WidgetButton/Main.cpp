// $Revision: 1.14 $
#include "SmartWin.h"
#include <vector>
#include <assert.h>
using namespace SmartWin;

struct pairStyleDescription
{
	DWORD style;
	SmartUtil::tstring description;

	pairStyleDescription( const DWORD a_style, const SmartUtil::tstring & a_description )
		: style( a_style )
		, description( a_description )
	{}
};

pairStyleDescription listOfStyles[] =
{ pairStyleDescription( WS_VISIBLE | WS_TABSTOP | WS_GROUP, _T( "Default button" ) )
, pairStyleDescription( WS_VISIBLE | WS_TABSTOP | BS_3STATE, _T( "3 state button" ) )
, pairStyleDescription( WS_VISIBLE | WS_TABSTOP | BS_AUTO3STATE, _T( "3 state auto check" ) )
, pairStyleDescription( WS_VISIBLE | WS_TABSTOP | BS_AUTOCHECKBOX, _T( "Auto check box" ) )
, pairStyleDescription( WS_VISIBLE | WS_TABSTOP | BS_AUTORADIOBUTTON, _T( "Auto radio box" ) )
, pairStyleDescription( WS_VISIBLE | WS_TABSTOP | BS_BOTTOM, _T( "Bottom text" ) )
, pairStyleDescription( WS_VISIBLE | WS_TABSTOP | BS_CENTER, _T( "Centered text" ) )
, pairStyleDescription( WS_VISIBLE | WS_TABSTOP | BS_CHECKBOX, _T( "Checkbox" ) )
, pairStyleDescription( WS_VISIBLE | WS_TABSTOP | BS_DEFPUSHBUTTON, _T( "Default push" ) )
, pairStyleDescription( WS_VISIBLE | WS_TABSTOP | BS_FLAT, _T( "Flat style" ) )
, pairStyleDescription( WS_VISIBLE | WS_TABSTOP | BS_GROUPBOX, _T( "Group box" ) )
, pairStyleDescription( WS_VISIBLE | WS_TABSTOP | BS_LEFT, _T( "Left" ) )
, pairStyleDescription( WS_VISIBLE | WS_TABSTOP | BS_LEFTTEXT, _T( "Left Text" ) )
, pairStyleDescription( WS_VISIBLE | WS_TABSTOP | BS_MULTILINE, _T( "Short line\nLong line (automatically split) in a multiline button" ) )
, pairStyleDescription( WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON, _T( "Push button" ) )
, pairStyleDescription( WS_VISIBLE | WS_TABSTOP | BS_PUSHLIKE, _T( "Push like" ) )
, pairStyleDescription( WS_VISIBLE | WS_TABSTOP | BS_RADIOBUTTON, _T( "Radio button" ) )
, pairStyleDescription( WS_VISIBLE | WS_TABSTOP | BS_RIGHT, _T( "Right button" ) )
, pairStyleDescription( WS_VISIBLE | WS_TABSTOP | BS_RIGHTBUTTON, _T( "Right Text" ) )
, pairStyleDescription( WS_VISIBLE | WS_TABSTOP | BS_TEXT, _T( "Text" ) )
, pairStyleDescription( WS_VISIBLE | WS_TABSTOP | BS_TOP, _T( "Top" ) )
, pairStyleDescription( WS_VISIBLE | WS_TABSTOP | BS_USERBUTTON, _T( "User button" ) )
, pairStyleDescription( WS_VISIBLE | WS_TABSTOP | BS_VCENTER, _T( "Vertically centered" ) )
};

SmartWin::Rectangle & calculatePosition()
{
	static unsigned int d_index = 0;
	static unsigned int d_boxsizeh = 140;
	static unsigned int d_boxsizev = 50;
	static SmartWin::Rectangle d_answer;

	d_answer.pos.x = 10 + ( d_index % 5 ) * d_boxsizeh;
	d_answer.pos.y = 10 + ( d_index++ / 5 ) * d_boxsizev;
	d_answer.size.x = d_boxsizeh - 10;
	d_answer.size.y = d_boxsizev - 10;
	return d_answer;
}

// The "factory" version
class WidgetTest1
	: public WidgetFactory< WidgetWindow, WidgetTest1 >
{
private:
	WidgetButtonPtr button;
public:
	WidgetTest1()
	{}

	void clicked( WidgetButtonPtr button )
	{
		int no = 0;
		fork< int >( no, & WidgetTest1::threadProc );
		createMessageBox().show( button->getText() );
	}

	bool closing()
	{
		// Can't close before thread finishes...
		if ( getCriticalSection().isLocked() )
			return false;
		return true;
	}

	void createFromPairStyleDescription( const int a_idx )
	{
		WidgetButtonPtr d_button;
		WidgetButton::Seed buttonCS;

		buttonCS.style = listOfStyles[ a_idx ].style;
		buttonCS.location = calculatePosition();
		buttonCS.caption = listOfStyles[ a_idx ].description;
		d_button = createButton( buttonCS );
	}

	void initAndCreate()
	{
		WidgetWindow::Seed windowCS;

		// Uncomment the following code to get the blue background shown in the
		// screenshoots. Currently, only the background of the main window can be
		// changed using Seed.
		
		// windowCS.background = CreateSolidBrush( RGB( 0, 0, 125 ) );
		windowCS.caption = _T( "WidgetButton compile and execute test" );
		windowCS.location = SmartWin::Rectangle( 100, 100, 720, 350 );
		createWindow( windowCS );

		for ( int i = 0; i < 23; ++i )
			createFromPairStyleDescription( i );

		WidgetButton::Seed buttonCS;

		calculatePosition();
		calculatePosition();
		buttonCS.location.pos = calculatePosition().pos;
		buttonCS.location.size = calculatePosition().size;
		buttonCS.location.size.x *= 2;
		buttonCS.font = createFont( _T( "Times New Roman" ), 0, 10, 1, ANSI_CHARSET, true, false, false, 0, OUT_DEFAULT_PRECIS );
		buttonCS.caption = _T( "Push me, I'll fork" );
		button = createButton( buttonCS );
		assert( button->getText() == _T( "Push me, I'll fork" ) );
		button->onClicked( & WidgetTest1::clicked );
		button->setVisible( true );
		onClosing( & WidgetTest1::closing );
	}

	unsigned long threadProc( int & no )
	{
		// Need a lock here since we need to determine later if we can close Widget
		// or not... (Can't close before threadPorc finishes...) Also need to
		// disable button to make sure we don't come here twice...
		button->setEnabled( false );
		Utilities::ThreadLock tmp( getCriticalSection() );
		int idx = 0;
		while ( idx++ < 100 )
		{
			SmartUtil::tstring txt = getText();
			txt = txt.substr( 1 ) + txt.substr( 0, 1 );
			setText( txt );
			Sleep( 50 );
		}
		button->setEnabled( true );
		return 0;
	}
};

void clicked( WidgetTest1 * parent, WidgetTest1::WidgetButtonPtr button )
{
	WidgetTest1::WidgetMessageBox box = parent->createMessageBox();
	box.show( button->getText() );
	button->setEnabled( false );
}

// Unit test for WidgetButton, uses some of the functions/constructs available in the WidgetButton
int SmartWinMain( Application & app )
{
	// Widget (Factory) version
	WidgetTest1 * testWnd1 = new WidgetTest1;
	testWnd1->initAndCreate();

	// Creating "free" button...
	WidgetTest1::WidgetButtonPtr button = testWnd1->createButton();
	calculatePosition();
	button->setBounds( calculatePosition() );
	button->setText( _T( "I'll become disabled" ) );
	button->onClicked( clicked );
	return app.run();
}
