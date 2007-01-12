// $Revision: 1.12 $
// $Revision : $
/*!
  \file Main.cpp
  \brief WidgetTextBox sample
*/
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
{ pairStyleDescription( WS_VISIBLE | WS_TABSTOP | WS_GROUP, _T( "Default text" ) )
, pairStyleDescription( WS_VISIBLE | WS_TABSTOP | ES_PASSWORD, _T( "Passworded text" ) )
, pairStyleDescription( WS_VISIBLE | WS_TABSTOP | ES_CENTER, _T( "Centered text" ) )
, pairStyleDescription( WS_VISIBLE | WS_TABSTOP | ES_LEFT, _T( "Left Text" ) )
, pairStyleDescription( WS_VISIBLE | WS_TABSTOP | ES_LOWERCASE, _T( "Lowercase text" ) )
, pairStyleDescription( WS_VISIBLE | WS_TABSTOP | ES_MULTILINE, _T( "Multiline text ... " )
		_T( "TAB doesn't go past here ... try pressing TAB while in any other textbox" ) )
, pairStyleDescription( WS_VISIBLE | WS_TABSTOP | ES_NOHIDESEL, _T( "Don't hide selection" ) )
, pairStyleDescription( WS_VISIBLE | WS_TABSTOP | ES_NUMBER, _T( "012345657890" ) )
, pairStyleDescription( WS_VISIBLE | WS_TABSTOP | 0, _T( "012345657890." ) )
, pairStyleDescription( WS_VISIBLE | WS_TABSTOP | ES_UPPERCASE, _T( "Uppercase text" ) )
, pairStyleDescription( WS_VISIBLE | WS_TABSTOP | ES_READONLY, _T( "Read only" ) )
};

SmartWin::Rectangle & calculatePosition()
{
	static unsigned int d_index = 0;
	static unsigned int d_boxsize = 60;
	static SmartWin::Rectangle d_answer;

	int rows = 3, cols = 4;

	d_answer.pos.x = 10 + ( d_index % cols ) * rows * d_boxsize;
	d_answer.pos.y = 10 + ( d_index++ / cols ) * d_boxsize;
	d_answer.size.x = rows * d_boxsize - 10;
	d_answer.size.y = d_boxsize * 9 / 10;
	return d_answer;
}

// The "factory" version
class WidgetTest1
	: public WidgetFactory< WidgetWindow, WidgetTest1 >
{
private:
	std::vector < WidgetTextBox * > textBoxs;
public:
	WidgetTest1()
	{}

	void txtUpdated( WidgetTextBoxPtr box )
	{
		setText( box->getText() );
	}

	void createFromPairStyleDescription( const int a_idx )
	{
		WidgetTextBoxPtr textBox;
		WidgetTextBox::Seed textBoxCS;

		textBoxCS.style = listOfStyles[ a_idx ].style;
		textBoxCS.location = calculatePosition();
		textBoxCS.caption = listOfStyles[ a_idx ].description;
		textBox = createTextBox( textBoxCS );
		textBox->onUpdate( & WidgetTest1::txtUpdated );
		if (a_idx % 4 == 0)
			textBox->setBorder();
		else if (a_idx % 4 == 1)
			textBox->setSunkenBorder();
		else if (a_idx % 4 == 2)
			textBox->setSmoothSunkenBorder();
		else if (a_idx % 4 == 3)
			textBox->setRaisedBorder();
		textBoxs.push_back( textBox );
	}

	void initAndCreate()
	{
		WidgetWindow::Seed windowCS;
		SmartWin::Rectangle rect( 60, 60, 740, 230 );

		//non - resizable window
		windowCS.style ^= WS_SIZEBOX;
		windowCS.caption = _T( "WidgetTextBox compile and execute test" );
		windowCS.background = CreateSolidBrush( RGB( 0, 0, 125 ) );
		windowCS.location = rect;
		createWindow( windowCS );

		setBounds( rect ); //unnecessary (was set in the seed), just to test set/get symmetry
		assert( getBounds().pos == rect.pos && getBounds().size == rect.size );

		int numBoxes = sizeof( listOfStyles ) / sizeof( pairStyleDescription );
		for ( int i = 0; i < numBoxes; ++i )
			createFromPairStyleDescription( i );

		textBoxs[8]->onChar( & WidgetTest1::acceptOnlyNumeric );
		textBoxs[8]->setTextLimit( 17 );
		DWORD maxChars= textBoxs[8]->getTextLimit();
	}

	// Discard asll chars except 0-9, '-' and '.'
	// out: true if char was handled; ie that the system does not need to handle it further.
	//
	bool acceptOnlyNumeric( WidgetTextBoxPtr textField, int keychar )
	{
		if ( isdigit( keychar ) ) return false; // Accept 0-9
		switch ( keychar )
		{
			case '.' :
			case '-' :
			case VK_LEFT :
			case VK_RIGHT :
			case VK_BACK :
			return false; // Let control handle character
		}
		return true; // Throw the char away.
	}
};

// Unit test for WidgetTextBox, tries to instantiate all different forms of
// WidgetTextBox and also tries to use all functions
int SmartWinMain( Application & app )
{
	// Widget (Factory) version
	WidgetTest1 * testWnd1 = new WidgetTest1;
	testWnd1->initAndCreate();
	return app.run();
}
