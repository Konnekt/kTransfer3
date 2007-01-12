// $Revision: 1.8 $
#include "MainWindow.h"
#include "CentralWidget.h"

ABMainWindow::ABMainWindow()
{
	createWindow();
	setText( _T( "PPC - AddressBook" ) );
	setupMenuBar();
	setupCentralWidget();
	onSized( & ABMainWindow::sized );
	maximize();
	updateWidget();
}

void ABMainWindow::setupMenuBar()
{
	mainMenu = createMenu();

	WidgetMenuPtr file = mainMenu->appendPopup( _T( "&File" ) );

	file->appendItem( 1, _T( "&New" ), & ABMainWindow::menuEventHandler );
	WidgetMenuPtr loadSave = file->appendPopup( _T( "Open/Save" ) );
	loadSave->appendItem( 2, _T( "&Open" ), & ABMainWindow::menuEventHandler );
	loadSave->appendItem( 3, _T( "&Save" ), & ABMainWindow::menuEventHandler );
	loadSave->appendItem( 4, _T( "Save &as..." ), & ABMainWindow::menuEventHandler );
	file->appendSeparatorItem();
	file->appendItem( 5, _T( "&Print..." ), & ABMainWindow::menuEventHandler );
	file->appendSeparatorItem();
	file->appendItem( 6, _T( "&Close" ), & ABMainWindow::menuEventHandler );
	file->appendItem( 7, _T( "&Quit" ), & ABMainWindow::menuEventHandler );

	// Appending checked item for SIP
	mainMenu->appendItem( 0, _T( "Keyboard" ), & ABMainWindow::menuEventHandler );

	// Disabling the menu item "New" since we don't want to support multiple documents
	file->setItemEnabled( 1, false );
	file->setItemEnabled( 2, false );
}

void ABMainWindow::setupCentralWidget()
{
	central = new ABCentralWidget( this );
	mainMenu->setItemEnabled( 1, false );
	mainMenu->setItemEnabled( 2, false );
	mainMenu->setItemEnabled( 3, true );
	mainMenu->setItemEnabled( 4, true );
	mainMenu->setItemEnabled( 5, true );
	mainMenu->setItemEnabled( 6, true );
}

void ABMainWindow::menuEventHandler( WidgetMenuPtr menu, unsigned item )
{
	using namespace SmartWin;
	switch ( item )
	{
	case 0 :
		if ( Application::instance().getSipVisible() )
		{
			Application::instance().showSip( this, false );
		}
		else
		{
			Application::instance().showSip( this, true );
		}
		break;
	case 1 :
		setupCentralWidget();
		break;
	case 2 :
		{
			WidgetLoadFile dlg = createLoadFile();
			dlg.addFilter( _T( "Text files" ), _T( "*.txt" ) );
			SmartUtil::tstring path = dlg.showDialog();
			if ( path.size() > 0 )
			{
				setupCentralWidget();
				central->load( path );
				filename = path;
			}
		} break;
	case 3 :
		if ( filename.size() != 0 )
		{
			central->save( filename );
			break;
		}
		// INTENTIONALLY fall through to "Save As"...
	case 4 :
		{
			WidgetSaveFile dlg = createSaveFile();
			dlg.addFilter( _T( "Text files" ), _T( "*.txt" ) );
			SmartUtil::tstring path = dlg.showDialog();
			if ( path.size() > 0 )
			{
				filename = path;
				central->save( filename );
			}
		} break;
	case 5 :
		break; // In the Qt AddressBook sample "print" is never implemented!  ( ...puhh!... ;)
	case 6 :
		central->close();
		central = 0;

		// Disabling menu items dependant on central widget being alive...
		menu->setItemEnabled( 1, true );
		menu->setItemEnabled( 2, true );
		menu->setItemEnabled( 3, false );
		menu->setItemEnabled( 4, false );
		menu->setItemEnabled( 5, false );
		menu->setItemEnabled( 6, false );
		break;
	case 7 :
		close();
		return;
	}
}

void ABMainWindow::sized( const SmartWin::WidgetSizedEventResult & sz )
{
	if ( central )
	{
		SmartWin::Rectangle rc = sw::Rectangle( getClientAreaSize() );
		central->setBounds( 0, 0, rc.size.x, rc.size.y );
	}
}
