// $Revision: 1.12 $
/*!
  \file WidgetMain.cpp
  \brief 
*/

#include <algorithm>
#include "WidgetMain.h"
#include "MDIChild.h"
#include "resource.h"

void WidgetMain::sized( const SmartWin::WidgetSizedEventResult & sz )
{
	mdiClient->setBounds( 10, 130, sz.newSize.x - 20, sz.newSize.y - 160 );
	status->refresh();
	toolbar->refresh();
}

SmartUtil::tstring WidgetMain::outputNumberOfMdiWidgets()
{
	SmartUtil::tstring tmp = boost::lexical_cast< SmartUtil::tstring >( static_cast< unsigned >( mdiWindows.size() ) );
	status->setText( tmp );
	return _T( "WidgetChild : " ) + tmp;
}

void WidgetMain::newWindow( WidgetButtonPtr btn )
{
	MDIChild * mdi = new MDIChild( this, mdiClient );
	mdiWindows.push_back( mdi );
	SmartUtil::tstring name = outputNumberOfMdiWidgets();
	mdi->setText( name );
}

void WidgetMain::toolbarClicked( WidgetToolbarPtr toolbar, unsigned btn )
{
}

void WidgetMain::mdiClosed( MDIChild * mdi )
{
	std::vector < MDIChild * >::iterator toBeRemoved = std::find( mdiWindows.begin(), mdiWindows.end(), mdi );
	mdiWindows.erase( toBeRemoved );
	outputNumberOfMdiWidgets();
}

void WidgetMain::initAndCreate()
{
	// Create main
	WidgetWindow::Seed windowCS;

	windowCS.location = SmartWin::Rectangle( 100, 100, 600, 600 );
	windowCS.background = CreateSolidBrush( RGB( 0, 0, 125 ) );
	windowCS.caption = _T( "WidgetMDIParent compile and execute test" );
	createWindow( windowCS );
	onSized( & WidgetMain::sized );

	WidgetButtonPtr btn = createButton();
	btn->setText( _T( "New Window" ) );
	btn->setBounds( 200, 100, 100, 20 );
	btn->onClicked( & WidgetMain::newWindow );

	ImageListPtr imageList( new ImageList( 32, 32, ILC_COLOR24 ) );
	imageList->addMultiple( SmartWin::Bitmap( IDB_BITMAP1 ) );

	status = createStatusBar();
	toolbar = createToolbar();
	toolbar->setButtonSize( 48, 48 );
	toolbar->setNormalImageList( imageList );
	toolbar->addButton( 0, 0, _T( "Test" ), _T( "Testing" ), false, & WidgetMain::toolbarClicked );
	toolbar->addButton( 1, 1, _T( "Test" ), _T( "Testing" ), false, & WidgetMain::toolbarClicked );
	toolbar->refresh();

	WidgetStaticPtr stat = createStatic();
	stat->setText( _T( "Try to type into the text box within the MDI and close it afterwards" ) );
	stat->setBounds( 310, 100, 300, 30 );

	// Create MDI Client
	WidgetMDIParent::Seed cs;

	cs.location = SmartWin::Rectangle( 10, 130, 570, 380 );
	mdiClient = createMDIParent( cs );

	// Resizing to kick in event onSized
	//setBounds( 100, 100, 600, 600 );
}
