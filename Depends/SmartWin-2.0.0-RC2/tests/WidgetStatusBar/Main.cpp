// $Revision: 1.10 $
/*!
  \file Main.cpp
  \brief
*/

#include "SmartWin.h"
#include <vector>
#include <assert.h>
using namespace SmartWin;

class WidgetTest2
	: public WidgetFactory< WidgetWindow, WidgetTest2 >
{
	WidgetStatusBarSectionsPtr stat;
public:
	void sized( const SmartWin::WidgetSizedEventResult & sz )
	{
		stat->refresh();
	}

	void mouseMoved( WidgetStatusBarSectionsPtr status, const MouseEventResult & res )
	{
		SmartUtil::tstring tmp = _T( "Mouse moved : " );
		tmp += boost::lexical_cast< SmartUtil::tstring >( res.pos.x );
		tmp += _T( ", " );
		tmp += boost::lexical_cast< SmartUtil::tstring >( res.pos.y );
		this->setText( tmp );
	}

	void rightClicked( WidgetStatusBarSectionsPtr stat )
	{
		this->setText( _T( "Mouse right clicked" ) );
	}

	void initAndCreate()
	{
		WidgetWindow::Seed windowCS;

		windowCS.location = SmartWin::Rectangle( 150, 90, 500, 220 );
		windowCS.background = CreateSolidBrush( RGB( 0, 128, 125 ) );
		windowCS.caption = _T( "WidgetStatusBar compile and execute test WITHOUT sections" );
		createWindow( windowCS );
		onSized( & WidgetTest2::sized );

		// Creating a status bar WITH SECTIONS
		WidgetStatusBarSections::Seed widgetStatusBarSectionsCS;

		widgetStatusBarSectionsCS.font = createFont( _T( "Microsoft Sans Serif" ), 0, 10, 1, ANSI_CHARSET, true, false, false, 0, OUT_DEFAULT_PRECIS );
		stat = createStatusBarSections( widgetStatusBarSectionsCS );
		std::vector< unsigned > sections;
		sections.push_back( 150 );
		sections.push_back( 150 );
		sections.push_back( 150 );
		stat->setSections( sections );
		stat->setText( _T( "Part1" ), 0 );
		stat->setText( _T( "Part2" ), 1 );
		stat->setText( _T( "Part3" ), 2 );
		stat->onMouseMove( & WidgetTest2::mouseMoved );
		stat->onRightClicked( & WidgetTest2::rightClicked );
	}
};

// The "factory" version
class WidgetTest1
	: public WidgetFactory< WidgetWindow, WidgetTest1 >
{
	WidgetStatusBarPtr stat;
public:
	WidgetTest1()
	{}

	void clicked( WidgetStatusBarPtr stat )
	{
		stat->setVisible( false );
		createMessageBox().show( stat->getText() );
		stat->setVisible( true );
	}

	void sized( const SmartWin::WidgetSizedEventResult & sz )
	{
		stat->refresh();
	}

	void painting( WidgetStatusBarPtr stat, Canvas & canvas )
	{
		Brush brush( canvas, RGB( 0, 255, 0 ) );
		FontPtr font = stat->getFont();
		canvas.selectFont( font );
		canvas.fillRectangle( sw::Rectangle( stat->getClientAreaSize() ), brush );
		canvas.setBkMode( true );
		canvas.drawText( stat->getText(), sw::Rectangle( stat->getClientAreaSize() ), DT_CENTER );
	}

	void initAndCreate()
	{
		WidgetWindow::Seed windowCS;

		windowCS.location = SmartWin::Rectangle( 50, 50, 400, 300 );
		windowCS.background = CreateSolidBrush( RGB( 0, 0, 125 ) );
		windowCS.caption = _T( "WidgetStatusBar compile and execute test" );
		createWindow( windowCS );

		WidgetStatusBar::Seed statusBarCS;

		statusBarCS.caption = _T( "Tomasito" );
		statusBarCS.font = createFont( _T( "Times New Roman" ), 0, 10, 1, ANSI_CHARSET, true, false, false, 0, OUT_DEFAULT_PRECIS );
		stat = createStatusBar( statusBarCS );
		stat->onPainting( & WidgetTest1::painting );

		onSized( & WidgetTest1::sized );
		stat->onDblClicked( & WidgetTest1::clicked );
		stat->refresh();
	}
};

int SmartWinMain( Application & app )
{
	WidgetTest1 * testWnd1 = new WidgetTest1;
	testWnd1->initAndCreate();

	WidgetTest2 * testWnd2 = new WidgetTest2;
	testWnd2->initAndCreate();

	return app.run();
}
