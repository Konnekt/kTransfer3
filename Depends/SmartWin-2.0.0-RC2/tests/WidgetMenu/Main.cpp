// $Revision: 1.15 $
/*!
  \file Main.cpp
  \brief 
*/
// Menu functions
//
// Unit tests try to instantiate all different forms and use all functions.
//

#include "SmartWin.h"
#include <vector>
#include <assert.h>
using namespace SmartWin;

// The "factory" version
class WidgetTest1
	: public WidgetFactory< WidgetWindow, WidgetTest1 >
{
	// We must keep a reference to the menu object otherwise it's being "collected" and we "lose" it...
	WidgetMenuPtr mainMenu;
	WidgetMenuPtr recentFiles;
	WidgetMenuExtendedPtr extendedMenu;
	WidgetMenuExtendedPtr extendedFileMenu;
	WidgetButtonPtr button;

	bool extendedMenuIsAttahced; // this will show if current menu is extended or no

public:
	WidgetTest1()
	{}

	void extendedMenuEventHandler( WidgetMenuExtendedPtr menu, unsigned item )
	{
		if ( ( item == 14 ) || ( item == 15 ) )
			menu->checkItem( item, !menu->isItemChecked( item ), false );

		createMessageBox().show( menu->getItemText( item ) );
	}

	void menuEventHandler( WidgetMenuPtr menu, unsigned item )
	{
		if ( item == 6 )
		{
			menu->checkItem( item, !menu->getCheckedState( item ) );
		}
		createMessageBox().show( menu->getText( item ) );
	}

	void menuExitHandler( WidgetMenuPtr menu, unsigned item )
	{
		this->close();
	}

	void popupMenuViaRightClick( const SmartWin::MouseEventResult & mouse )
	{
		if ( extendedMenuIsAttahced )
			extendedFileMenu->trackPopupMenu( this );
		else
			recentFiles->trackPopupMenu( this );
	}

	void buttonClicked( WidgetButton * btn )
	{
		if ( btn->getText() == _T( "Press me to get NORMAL menu type" ) )
		{
			btn->setText( _T( "Press me to get NEW menu type" ) );
			mainMenu->attach( this );
		}
		else
		{
			btn->setText( _T( "Press me to get NORMAL menu type" ) );
			extendedMenu->attach( this );
		}

		extendedMenuIsAttahced = !extendedMenuIsAttahced;
	}

	void initAndCreate()
	{
		WidgetWindow::Seed windowCS;

		windowCS.location = SmartWin::Rectangle( 100, 100, 440, 400 );
		windowCS.caption = _T( "WidgetMenu compile and execute test" );
		createWindow( windowCS );

		WidgetStaticPtr stat = createStatic();
		stat->setBounds( 40, 30, 350, 40 );
		stat->setText( _T( "Don't forget to right click in the window or try the system menu" ) );

		// Creating button for switching menus
		button = createButton();
		button->setBounds( 40, 80, 300, 200 );
		button->setText( _T( "Press me to get NORMAL menu type" ) );
		button->onClicked( & WidgetTest1::buttonClicked );

		// Creating main menu
		mainMenu = createMenu();
		WidgetMenuPtr file = mainMenu->appendPopup( _T( "&File" ) );
		file->appendItem( 1, _T( "&Load" ), & WidgetTest1::menuEventHandler );
		file->appendItem( 2, _T( "&Save" ), & WidgetTest1::menuEventHandler );

		// Creating another popup UNDERNEATH mainMenu
		recentFiles = file->appendPopup( _T( "&Recent files" ) );
		recentFiles->appendItem( 3, _T( "&1. svada1.txt" ), & WidgetTest1::menuEventHandler );
		recentFiles->appendItem( 4, _T( "&2. svada2.txt" ), & WidgetTest1::menuEventHandler );
		recentFiles->setItemEnabled( 4, false );
		recentFiles->appendItem( 5, _T( "&3. svada3.txt" ), & WidgetTest1::menuEventHandler );

		// Adding more items underneath file menu
		file->appendSeparatorItem();
		file->appendCheckedItem( 6, _T( "&Check this..." ), & WidgetTest1::menuEventHandler );
		file->appendItem( 7, _T( "&Don't forget to right click in the window" ), & WidgetTest1::menuEventHandler );
		file->appendItem( 8, _T( "Exit" ), & WidgetTest1::menuExitHandler );

		// Creating extended menu
		extendedMenu = createExtendedMenu();

		// Checked item data
		MenuItemDataPtr checkedData( new MenuItemData() );
		checkedData->Font = FontPtr( new Font( _T( "Times New Roman" ), 28, 22, 2, ANSI_CHARSET, true ) );
		checkedData->Image = BitmapPtr( new Bitmap( _T( "images/help.bmp" ) ) );

		MenuItemDataPtr data( new MenuItemData() );
		extendedFileMenu = extendedMenu->appendPopup( _T( "&File" ), data );
		extendedFileMenu->setTitle( _T( "File Menu!" ) );
		FontPtr headerFont( new Font( _T( "Comic Sans MS" ), 32, 22, 4 ) );
		extendedFileMenu->setTitleFont( headerFont );

		MenuItemDataPtr loadData( new MenuItemData() );
		loadData->Image = BitmapPtr( new Bitmap( _T( "images/open.bmp" ) ) );
		loadData->TextColor = RGB( 255, 0, 255 );
		extendedFileMenu->appendItem( 11, _T( "&Load\tCtrl + L" ), loadData, & WidgetTest1::extendedMenuEventHandler );

		MenuItemDataPtr saveData( new MenuItemData() );
		saveData->Image = BitmapPtr( new Bitmap( _T( "images/save.bmp" ) ) );
		extendedFileMenu->appendItem( 12, _T( "&Save\tCtrl + S" ), saveData, & WidgetTest1::extendedMenuEventHandler );

		// create extended recent files menu
		WidgetMenuExtendedPtr extendedRecentFiles = extendedFileMenu->appendPopup( _T( "&Recent Files" ), data );

		MenuItemDataPtr disabledData( new MenuItemData() );
		disabledData->Image = BitmapPtr( new Bitmap( _T( "images/delete.bmp" ) ) );
		extendedRecentFiles->appendItem( 13, _T( "No recent files" ), disabledData, & WidgetTest1::extendedMenuEventHandler );
		extendedRecentFiles->enableItem( 13, false );

		// add more items
		extendedFileMenu->appendSeparatorItem();
		extendedFileMenu->appendItem( 14, _T( "&Check this..." ), data, & WidgetTest1::extendedMenuEventHandler );
		extendedFileMenu->checkItem( 14, true, false );

		extendedFileMenu->appendItem( 15, _T( "&And this..." ), checkedData, & WidgetTest1::extendedMenuEventHandler );
		extendedFileMenu->checkItem( 15, true, false );

		// Now we're going TRULY bananas here just to demonstrate the concept!!
		// Obviously this is gonna look like totally CRAP but the idea is the POSSIBILITIES and not our design skills...!     ;)
		MenuColorInfo colorInfo = extendedFileMenu->getColorInfo();
		colorInfo.colorImageBackground = RGB( 192, 192, 192 );
		colorInfo.colorHighlight = RGB( 92, 0, 0 );
		colorInfo.colorMenu = RGB( 192, 255, 232 );
		colorInfo.colorMenuBar = RGB( 255, 0, 0 );
		colorInfo.colorStrip = RGB( 162, 225, 202 );
		colorInfo.colorTitle = RGB( 198, 67, 232 );
		colorInfo.colorTitleText = RGB( 108, 7, 192 );
		extendedFileMenu->setColorInfo( colorInfo );
		extendedRecentFiles->setColorInfo( colorInfo );

		// Extended system menu
		WidgetMenuExtendedPtr sysMenu = extendedMenu->getSystemMenu();

		// set system menu sidebar
		sysMenu->setTitle( _T("SysMenu!"), true /* Draw as sidebar */ );

		// Attach to this widget
		extendedMenu->attach( this );
		extendedMenuIsAttahced = true;

		// Function to do TrackPopupMenu
		onRightMouseDown( & WidgetTest1::popupMenuViaRightClick );
	}
};

int SmartWinMain( Application & app )
{
	// Widget (Factory) version
	WidgetTest1 * testWnd1 = new WidgetTest1;
	testWnd1->initAndCreate();
	return app.run();
}
