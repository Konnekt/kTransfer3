// $Revision: 1.17 $
/* Copyright (c) 2005 - Thomas Hansen
 * Part of the SmartWin++ library example code
 * Permission to use under the same terms as the actual library
 * The license to the library can be found at http://smartwinlib.org
 * License for library is Open Source and BSD
 */
#include "MainWindow.h"
#include "TabSheetWidget.h"
#include "resource.h"

using namespace SmartWin;

// This is an example of the "raw" Event Handler which can handle Events not by default "trappable" in SmartWin++
// Here you can trap ANYTHING your system can throw at you...  ;)
// Note!
// Since TabSheetWidget is responsible for handling it's own Events (it's its own Event Handler Widget) the first and the
// second argument here are basically the same pointer...
// If it wasn't inherited from WidgetTabSheet but directly embedded into the MainWindow as a WidgetTabSheet it would
// have been a MainWindow * we got as the first argument...
HRESULT genericEventHandler( TabSheetWidget * parent, TabSheetWidget::WidgetTabSheetPtr This, LPARAM lPar, WPARAM wPar )
{
	// gcc doesn't support NMTCKEYDOWN structure...
#ifndef __GNUC__
	parent->itsParent->setText( SmartUtil::tstring( _T( "Button pressed: " ) ) + boost::lexical_cast< SmartUtil::tstring >( ( int )( ( NMTCKEYDOWN * ) lPar )->wVKey ) );
#endif
	return 0;
}

MainWindow::MainWindow()
		: Widget()
{
}

void MainWindow::initAndCreate()
{
	// creating and initializing main window
	WidgetWindow::Seed cs;

	cs.background = CreateSolidBrush( RGB( 0, 0, 125 ) );
	cs.style |= WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME;
	createWindow( cs );
	setIconSmall( IDI_ICON1 );
#ifdef WINCE
	sw::Rectangle rc = sw::Rectangle( getDesktopSize() );
#else
	sw::Rectangle rc( 100, 100, 300, 320 );
#endif
	rc = rc.shrink( 5, 20 );
	rc.pos.y += 15;
	this->setBounds( rc );
	setText( _T( "Sample Widgets" ) );
	onSized( & MainWindow::sized );

	// Creating tab sheets
	itsTabSheet = new TabSheetWidget( this );
	itsTabSheet->initAndCreate();

	// Setting our "raw" Event Handler...
	itsTabSheet->onRaw( & genericEventHandler, Message( WM_NOTIFY, TCN_KEYDOWN ) );
	updateWidget();
}

void MainWindow::sized( const WidgetSizedEventResult & newSize )
{
	itsTabSheet->layout();
}
