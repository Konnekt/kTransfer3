// $Revision: 1.13 $
/*!
  \file Main.cpp
  \brief Unit test for WidgetStatic

  It tries to instantiate all different forms of WidgetStatic and it also tries to use all functions.

  TODO: If you resize quicly the window, the static control doesn't repaint completely ...
*/

#include "SmartWin.h"
#include <vector>
#include <assert.h>
#include <time.h>
using namespace SmartWin;

// The "factory" version
class WidgetTest1
	: public WidgetFactory< WidgetWindow, WidgetTest1 >
{
public:
	WidgetTest1()
	{}

	void dblClicked( WidgetStaticPtr stat )
	{
		createMessageBox().show( SmartUtil::tstring( _T( "Static Widget double clicked : " ) ) + stat->getText() );
	}

	void textChanged( WidgetStaticPtr stat, const SmartUtil::tstring & text )
	{
		createMessageBox().show( SmartUtil::tstring( _T( "Text changed Static Widget : " ) ) + text );
	}

	void textChanged( const SmartUtil::tstring & text )
	{
		createMessageBox().show( SmartUtil::tstring( _T( "Text changed Main Widget : " ) ) + text );
	}

	void initAndCreate()
	{
		WidgetWindow::Seed windowCS;

		windowCS.location = SmartWin::Rectangle( 0, 50, 300, 300 );
		windowCS.background = CreateSolidBrush( RGB( 0, 0, 125 ) );
		windowCS.caption = _T( "WidgetStatic test" );
		createWindow( windowCS );
		onTextChanging( & WidgetTest1::textChanged );

		WidgetStatic::Seed staticCs;

		staticCs.caption = _T( "Initial text will be changed to test handler." );
		staticCs.font = createFont( _T( "Times New Roman" ), 0, 10, 1, ANSI_CHARSET, true, false, false, 0, OUT_DEFAULT_PRECIS );
		staticCs.location = SmartWin::Rectangle( getClientAreaSize() ).shrink( 50l );
		WidgetStaticPtr stat = createStatic( staticCs );
		srand( ( unsigned ) time( NULL ) );
		stat->onBackgroundColor( & WidgetTest1::backgroundColor );
		stat->onTextChanging( & WidgetTest1::textChanged );
		stat->onDblClicked( & WidgetTest1::dblClicked );

		stat->setText( _T( "Testing static control ... \nthe background and the font color will change...\nTry resizing the window quicly" ) );

		//some compile tests and get/set symmetry test
		stat->setBounds( staticCs.location, false );
		assert( stat->getBounds().pos == staticCs.location.pos && stat->getBounds().size == staticCs.location.size );
		assert( stat->getPosition() == staticCs.location.pos );
		assert( stat->getSize() == staticCs.location.size );
	}

	BrushPtr bgBrush;

	boost::shared_ptr< sw::Brush > backgroundColor( WidgetStaticPtr stat, SmartWin::Canvas & canvas )
	{
		// tranparent background for text
		canvas.setBkMode( true );
		SetTextColor( canvas.getDc(), RGB( rand()%255, rand()%255, rand()%255 ) );
		bgBrush = boost::shared_ptr< sw::Brush >( new SmartWin::Brush( canvas, RGB( rand()%255, rand()%255, rand()%255 ) ) );
		return bgBrush;
	}
};

int SmartWinMain( Application & app )
{
	// Widget (Factory) version
	WidgetTest1 * testWnd1 = new WidgetTest1;
	testWnd1->initAndCreate();
	return app.run();
}
