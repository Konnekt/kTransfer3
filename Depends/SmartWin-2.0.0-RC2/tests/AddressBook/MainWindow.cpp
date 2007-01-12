// $Revision: 1.14 $
/* Copyright (c) 2005 - Thomas Hansen
 * Part of the SmartWin++ library example code
 * Permission to use under the same terms as the actual library
 * The license to the library can be found at http://smartwinlib.org
 * License for library is Open Source and BSD
 */
#include "MainWindow.h"
#include "CentralWidget.h"

ABMainWindow::ABMainWindow()
{
	// In SmartWin++ you need to explicitly create the window, this is of course one more thing to remember
	// but at the same time you can also postpone window creation to when you feel for but still have the object itself
	// be created!

	WidgetWindow::Seed cs;

	createWindow( cs );
	setText( _T( "SmartWin++ Example - Addressbook" ) );
	setupMenuBar();
	setupCentralWidget();
	onSized( & ABMainWindow::sized );
	setBounds( 100, 100, 700, 600 );
}

void ABMainWindow::setupMenuBar()
{
	// In SmartWin++ you have the WidgetFactory which relieves you from explicitly defining the "this" argument to the Constructor
	// which makes it less prone to errors in most circumstances!
	itsMainMenu = createMenu();
	WidgetMenuPtr file = itsMainMenu->appendPopup( _T( "&File" ) );

	// In SmartWin++ you can REUSE the same event handler while you in Qt must have one Slot for each menu item
	// you can REUSE the even handler (if you want to) which makes the code smaller, more maintenable and less error prone!
	// Note though!
	// We COULD have different event handler if we WANTED to...
	file->appendItem( 1, _T( "&New" ), & ABMainWindow::menuEventHandler );
	file->appendItem( 2, _T( "&Open" ), & ABMainWindow::menuEventHandler );
	file->appendSeparatorItem();
	file->appendItem( 3, _T( "&Save" ), & ABMainWindow::menuEventHandler );
	file->appendItem( 4, _T( "Save &as..." ), & ABMainWindow::menuEventHandler );
	file->appendSeparatorItem();
	file->appendItem( 5, _T( "&Print..." ), & ABMainWindow::menuEventHandler );
	file->appendSeparatorItem();
	// In the Qt Address Book sample the Close and Quit does the same thing, we're gonna actually implement close here though
	file->appendItem( 6, _T( "&Close" ), & ABMainWindow::menuEventHandler );
	file->appendItem( 7, _T( "&Quit" ), & ABMainWindow::menuEventHandler );

	// In SmartWin++ you don't get a "menubar" object the way you do in Qt, therefore you have to explicitly "attach" the menu
	// to the Widget!
	itsMainMenu->attach( this );

	// Disabling the menu item "New" since we don't want to support multiple documents
	file->setItemEnabled( 1, false );
	file->setItemEnabled( 2, false );
}

void ABMainWindow::setupCentralWidget()
{
	itsCentral = new ABCentralWidget( this );
	setupMenuItems( true );
}

void ABMainWindow::setupMenuItems( bool central_widget_alive )
{
	itsMainMenu->setItemEnabled( 1, ! central_widget_alive );
	itsMainMenu->setItemEnabled( 2, ! central_widget_alive );

	for ( int item = 3; item <= 6; item++ )
		itsMainMenu->setItemEnabled( item, central_widget_alive );
}

// ONE event handler for all menu events (if you want to) which menu object is being pressed will be passed in
// as the parameter and which item being pressed in that menu is being passed in as an unsigned int
void ABMainWindow::menuEventHandler( WidgetMenuPtr menu, unsigned item )
{
	switch ( item )
	{
		case 1 :
			setupCentralWidget();
			break;
		case 2 :
			{
				WidgetLoadFile dlg = createLoadFile();
				SmartUtil::tstring path = dlg.showDialog();
				if ( path.size() > 0 )
				{
					setupCentralWidget();
					itsCentral->load( path );
					itsFilename = path;
				}
			}
			break;
		case 3 :
			if ( itsFilename.size() != 0 )
			{
				itsCentral->save( itsFilename );
				break;
			}
			// INTENTIONALLY fall through to "Save As"...
		case 4 :
			{
				WidgetSaveFile dlg = createSaveFile();
				SmartUtil::tstring path = dlg.showDialog();
				if ( path.size() > 0 )
				{
					itsFilename = path;
					itsCentral->save( itsFilename );
				}
			}
			break;
		case 5 :
			break; // In the Qt AddressBook sample "print" is never implemented!  ( ...puhh!... ;)
		case 6 :
			itsCentral->close();
			itsCentral = 0;

			// Disabling menu items dependant on central Widget being alive...
			setupMenuItems( false );
			break;
		case 7 :
			close();
			return;
	}
}

void ABMainWindow::sized( const sw::WidgetSizedEventResult & sz )
{
	if ( itsCentral )
	{
		// Also we need to accomodate for the size "by hand" which of course gives us more flexibility though...
		sw::Rectangle rc( getClientAreaSize() );

		// Just making it 100 pixels lesser on each side!
		itsCentral->setBounds( 0, 0, rc.size.x, rc.size.y );
	}
}
