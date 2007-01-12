// $Revision: 1.21 $
/*!
  \file MainWindow.cpp
  \brief
*/
#include "MainWindow.h"
#include "resource.h"
#include <string>
#include <sstream>
#include <fstream>

using namespace SmartWin;
using namespace std;
const unsigned int SCROLL_SPEED = 100;

const unsigned int MENU_FILE_OPEN = 1;
const unsigned int MENU_FILE_CLOSE = 2;
const unsigned int MENU_FILE_ABOUT = 3;
const unsigned int MENU_FILE_CLEAR = 4;
const unsigned int MENU_FILE_HELP = 5;
const unsigned int MENU_FILE_CHECK = 6;

#include "MainWindow.h"

// This is the one function EVERY project MUST have, note this is INSTEAD of WinMain function.
// Function must have EXACT same signature as this one, and it would often look exactly the same
// as this one, but could of course perform all sorts of different logic, doesn't have to
// do the same as this one...
int SmartWinMain( SmartWin::Application & app )
{
	MainWindow * main = new MainWindow();
	main->init();
	return app.run();
}

// Note!
// This is a global event handler, therefore we need to supply the window which the event is fired from as
// an input parameter, if it had on a "member" function we wouldn't have to do this...
void checkForAlarms( MainWindow * wnd, const CommandPtr & cmd )
{
	SmartUtil::tstring oldTxt = wnd->getText();
	SmartUtil::tstring newTxt = oldTxt.substr( 1 ) + oldTxt.substr( 0, 1 );
	wnd->setText( newTxt );
	SmartWin::Command com( _T( "CheckAlarms" ) );
	wnd->createTimer( checkForAlarms, SCROLL_SPEED, com );
}

void MainWindow::staticTextClicked( WidgetStaticPtr staticCtrl )
{
	WidgetMessageBox box = createMessageBox();
	box.show( _T( "You clicked me, look at the button ;)" ) );
	itsButton->setEnabled( !itsButton->getEnabled() );
	itsTxtField->setEnabled( !itsTxtField->getEnabled() );
}

void MainWindow::staticTextGenTre( WidgetStaticPtr staticCtrl )
{
	::ShellExecute( handle(), _T( "open" ), _T( "http : //www.geocities.com/polterguy1000"), 0, 0, SW_SHOWDEFAULT );
}

void MainWindow::statusBarDblClicked( WidgetStatusBarPtr statusBar )
{
	WidgetMessageBox box = createMessageBox();
	box.show( _T( "You DOUBLE clicked the statusbar, it contains the text : \"" ) + statusBar->getText() + _T( "\".\r\nWatch the Menu" ) );
	itsMenu->setItemEnabled( MENU_FILE_CHECK, !itsMenu->getItemEnabled( MENU_FILE_CHECK ) );
}

// Note!
// This is a "member" function, therefore we DON'T need to supply the window the event apply for as an argument
// as opposed to the above event handler which is "global".
void MainWindow::sized( const SmartWin::WidgetSizedEventResult & sz )
{
	// Resizing text field
	itsTxtField->setBounds( 0, 50, sz.newSize.x, 280 );

	// Whenever main window resizes its status bar must be resized (Windows API "bug")
	itsStatusBar->refresh();
	itsToolBar->refresh();
	updateStatusBar();
}

void MainWindow::moved( const SmartWin::Point & pt )
{
	updateStatusBar();
}

void MainWindow::updateStatusBar()
{
	SmartWin::Rectangle rect = getBounds();

	// Setting status bar text
	basic_stringstream< TCHAR > str;
	str << _T( "Size x : " ) << rect.size.x << _T( ",  size y : " ) << rect.size.y << _T( ", pos x : " ) << rect.pos.x << _T( ", pos y : " ) << rect.pos.y;
	itsStatusBar->setText( str.str() );
}

void MainWindow::chkClicked( WidgetCheckBoxPtr btn )
{
	WidgetMessageBox box = createMessageBox();
	if ( btn->getChecked() )
	{
		box.show( _T( "Checked!!" ) );
	}
	else
	{
		box.show( _T( "not checked..." ) );
	}
}

void MainWindow::treeViewClicked( WidgetTreeViewPtr view )
{
	WidgetMessageBox box = createMessageBox();
	box.show( _T( "Treeview clicked!!" ) );
}

void MainWindow::btnClickedNext( WidgetButtonPtr btn )
{
	SmartUtil::tstring poem = itsTxtField->getText();
	SmartUtil::tstring output;
	for ( SmartUtil::tstring::reverse_iterator idx = poem.rbegin();
		idx != poem.rend();
		++idx )
	{
		if ( * idx == '\r' )
			continue;
		if ( * idx == '\n' )
		{
			output += _T( "\r\n" );
		}
		else
			output.push_back( * idx );
	}
	itsTxtField->setText( output );
}
void MainWindow::btnClicked( WidgetButtonPtr btn )
{
	itsButton->setText( _T( "Push AGAIN!!" ) );
	SmartUtil::tstring story;
	story += _T( "A long, long time ago\r\n" );
	story += _T( "A son of a king, one of twelve in So\r\n" );
	story += _T( "Went out to win the world in his hand\r\n" );
	story += _T( "Came back with the story of a promised land\r\n" );
	story += _T( "He searched and he searched for twelve long days\r\n" );
	story += _T( "Eleven for his brothers and one for his face\r\n" );
	story += _T( "In the thirteenth day in the month of the Lord\r\n" );
	story += _T( "He found what he searched for a five pound sword\r\n" );
	story += _T( "This sword is made by an unknown source\r\n" );
	story += _T( "This sword is steared by hate, gold and force\r\n" );
	story += _T( "This sword is made by an unknown some\r\n" );
	story += _T( "This sword and he will make doomday come\r\n" );
	story += _T( "\r\n" );
	story += _T( "Poem by Thomas Hansen, written at an age of 16...;)\r\n" );
	itsTxtField->setText( story );

	// Note!
	// Here we are CHANGING the event handler...
	itsButton->onClicked( & MainWindow::btnClickedNext );
}

// Note that this example shows how to use a STATIC event handler. (It is TOTALLY transparent.)
// The only difference : we lack implicit access to the creational function from an object.
// We have for the case of the sample used the static Constructor of WidgetMessageBox here.
void MainWindow::toolbarClicked( MainWindow * parent, WidgetToolbarPtr item, unsigned int id )
{
	MainWindow::WidgetMessageBox box( parent );
	box.show( _T( "Toolbar clicked" ) );

	// Next line is an example of how we ALTERNATIVELY could have created a message box
	// This is a "safer" approach since we're guaranteed to get the "this" Widget as the parent.
	parent->createMessageBox().show( _T( "We could have done it like this too..." ) );

	// If this was NOT a static function but rather an object function of MainWindow we could have just called
	// "createMessageBox directly...

	if ( id == 2 )
		item->setButtonEnabled( 1, !item->getButtonEnabled( 1 ) );
	if ( id == 1 )
		item->setButtonVisible( 2, !item->getButtonVisible( 2 ) );
}

void MainWindow::menuClicked( WidgetMenuPtr item, unsigned int id )
{
	switch ( id )
	{
	case MENU_FILE_OPEN :
		{
			assert( item->getText( id ) == _T( "&Open" ) );
			WidgetLoadFile ptr = createLoadFile();
			SmartUtil::tstring fileName = ptr.showDialog();
			if ( fileName.size() > 0 )
			{
				basic_ifstream< TCHAR > file( SmartUtil::AsciiGuaranteed::doConvert( fileName.c_str(), SmartUtil::ConversionCodepage::ANSI ).c_str() );
				vector< char > fileContent;
				while ( true )
				{
					TCHAR ch;
					file.get( ch );
					if ( ch == _T( '\n' ) )
					{
						fileContent.push_back( _T( '\r' ) );
						fileContent.push_back( _T( '\n' ) );
					}
					else if ( ch != _T( '\r' ) )
						fileContent.push_back( ch );
					if ( file.eof() )
						break;
				}
				itsTxtField->setText( SmartUtil::tstring( fileContent.begin(), fileContent.end() ) );
			}
		} break;
	case MENU_FILE_CLOSE :
		{
			itsTxtField->setText( _T( "" ) );
		} break;
	case MENU_FILE_ABOUT :
		{
			WidgetMessageBox ptr = createMessageBox();
			ptr.show( _T( "ABOUT, note that the menus are SWITCHING... ;)" ) );
			itsMenu2->attach( this );
		} break;
	case MENU_FILE_CLEAR :
		{
			WidgetMessageBox ptr = createMessageBox();
			ptr.show( _T( "Changing back the menu..." ) );
			itsMenu->attach( this );
		} break;
	case MENU_FILE_HELP :
		{
			// Creating the dialog...
			itsDialogWidget = new ::DialogWidget( this );
		} break;
	case MENU_FILE_CHECK :
		{
			item->checkItem( MENU_FILE_CHECK, !item->getCheckedState( MENU_FILE_CHECK ) );
		} break;
	default:
		assert( false ); throw xCeption( _T( "Undefined Menu event id in event handler..." ) );
	}
}

bool MainWindow::listEdited( WidgetDataGridPtr list, unsigned col, unsigned row, SmartUtil::tstring & newVal )
{
	if ( row > 1 && row < 4 )
	{
		WidgetMessageBox box = createMessageBox();
		box.show( _T( "Uh, uh...\r\nYou can mess with me, you can even step on my blue sued shoes, but don't you DARE messing with SmartWin!" ) );
		return false;
	}
	return true;
}

void MainWindow::comboBoxSelChanged( WidgetComboBoxPtr cboBox )
{
	WidgetMessageBox box = createMessageBox();
	std::basic_stringstream< TCHAR > str;
	str << cboBox->getSelectedIndex();
	box.show( cboBox->getSelectedValue() + _T( "\r\n" ) + _T( "Index : " ) + str.str() );
}

bool MainWindow::closing()
{
	if ( itsTxtField->getText().size() != 0 )
	{
		WidgetMessageBox box = createMessageBox();
		if ( box.show( _T( "You have text in your area, are you sure you wish to close ? " ), _T( "Closing question" )
				, WidgetMessageBox::BOX_OKCANCEL, WidgetMessageBox::BOX_ICONQUESTION
				) != WidgetMessageBox::RETBOX_OK
		   )
			return false;
	}
	if ( itsTxtField->getText().size() != 0 && createMessageBox().show
		( _T( "Do you wish to save your content ? " ), _T( "Closing question" )
		, WidgetMessageBox::BOX_OKCANCEL, WidgetMessageBox::BOX_ICONQUESTION
		) == WidgetMessageBox::RETBOX_OK
	   )
	{
		// Note, this box is created "freely", we could have made it by calling Widget::createSaveFile
		WidgetSaveFile saveBox = createSaveFile();
		saveBox.addFilter( _T( "Text file" ), _T( "*.txt" ) );
		SmartUtil::tstring path = saveBox.showDialog();
		if ( path != _T( "" ) )
		{
			basic_ofstream< TCHAR > file( SmartUtil::AsciiGuaranteed::doConvert( path, SmartUtil::ConversionCodepage::ANSI ).c_str() );
			file << itsTxtField->getText();
		}
	}
	return true;
}

// Constructor of main window, note the calling of base class Constructor
MainWindow::MainWindow()
	// : WidgetFactory< WidgetWindow, MainWindow >( 0 )
{
}

void MainWindow::init()
{
	// Creating Main Widget
	createWindow();
	setVisible( true );
	setText( _T( "WidgetFun with SmartWin, the template based Windows GUI library...         " ) );
	setBounds( 100, 100, 500, 800 );
	onClosing( & MainWindow::closing );

	// Doing some FUN stuff...
	animateSlide( true, false, 300 );
	animateCollapse( false, 300 );
	animateBlend( true, 300 );

	// Creating our check box
	itsCheckBox = createCheckBox();
	itsCheckBox->setBounds( 10, 460, 200, 20 );
	itsCheckBox->setText( _T( "Try me!" ) );
	itsCheckBox->onClicked( & MainWindow::chkClicked );

	// Creating tree view
	WidgetTreeView::Seed treeSeed = WidgetTreeView::getDefaultSeed();
	treeSeed.exStyle = WS_EX_CLIENTEDGE;
	itsTree = createTreeView( treeSeed );
	itsTree->setHasButtons();
	itsTree->setHasLines();
	itsTree->setLinesAtRoot();
	itsTree->setEditLabels();
	itsTree->setBounds( 400, 450, 200, 200 );
	TreeViewNode root = itsTree->insertNode( _T( "A" ) );
	itsTree->insertNode( _T( "AA" ), root );
	TreeViewNode ab = itsTree->insertNode( _T( "AB" ), root );
	itsTree->insertNode( _T( "ABA" ), ab );
	TreeViewNode abb = itsTree->insertNode( _T( "ABB" ), ab );
	itsTree->insertNode( _T( "ABBA" ), abb );
	itsTree->insertNode( _T( "ABBB" ), abb );
	itsTree->insertNode( _T( "ABBC" ), abb );
	itsTree->insertNode( _T( "ABBD" ), abb );
	itsTree->insertNode( _T( "ABBE" ), abb );
	itsTree->insertNode( _T( "ABC" ), ab );
	TreeViewNode ac = itsTree->insertNode( _T( "AC" ), root );
	itsTree->insertNode( _T( "ACA" ), ac );
	itsTree->insertNode( _T( "ACB" ), ac );
	itsTree->insertNode( _T( "ACC" ), ac );
	itsTree->insertNode( _T( "ACD" ), ac );
	itsTree->onClicked( & MainWindow::treeViewClicked );

	// Creating list
	WidgetDataGrid::Seed gridSeed = WidgetDataGrid::getDefaultSeed();
	gridSeed.exStyle = WS_EX_CLIENTEDGE;
	itsList = createDataGrid( gridSeed );
	itsList->setBounds( 650, 450, 200, 100 );
	itsList->onValidate( & MainWindow::listEdited );
	itsList->setGridLines();
	itsList->setFullRowSelect();
	itsList->setHeaderDragDrop();
	itsList->setHoover();
	std::vector< SmartUtil::tstring > listVec;
	listVec.push_back( _T( "Col1" ) );
	listVec.push_back( _T( "Col2" ) );
	itsList->createColumns( listVec );
	listVec[0] = _T( "Thomas" );
	listVec[1] = _T( "Hansen" );
	itsList->insertRow( listVec );
	listVec[0] = _T( "er" );
	listVec[1] = _T( "tøff" );
	itsList->insertRow( listVec );
	listVec[0] = _T( "Smart" );
	listVec[1] = _T( "Win" );
	itsList->insertRow( listVec );
	listVec[0] = _T( "er" );
	listVec[1] = _T( "tøft" );
	itsList->insertRow( listVec );

	// Creating ComboBox
	itsBox = createComboBox();
	itsBox->setBounds( 0, 700, 100, 150 );
	itsBox->addValue( _T( "Thomas" ) );
	itsBox->addValue( _T( "Per" ) );
	itsBox->addValue( _T( "Helge" ) );
	itsBox->addValue( _T( "Jon" ) );
	itsBox->addValue( _T( "Erik" ) );
	itsBox->setSelectedIndex( 2 );
	itsBox->onSelectionChanged( & MainWindow::comboBoxSelChanged );

	// Creating default menu
	itsMenu = createMenu();
	WidgetMenuPtr file = itsMenu->appendPopup( _T( "&File" ) );
	file->appendItem( MENU_FILE_OPEN, _T( "&Open" ), & MainWindow::menuClicked );
	file->appendItem( MENU_FILE_CLOSE, _T( "&Close" ), & MainWindow::menuClicked );
	file = itsMenu->appendPopup( _T( "&Help" ) );
	file->appendItem( MENU_FILE_ABOUT, _T( "&About" ), & MainWindow::menuClicked );
	file = file->appendPopup( _T( "&Help Stuff" ) );
	file->appendItem( MENU_FILE_HELP, _T( "&Show Help" ), & MainWindow::menuClicked );
	file->appendSeparatorItem();
	file->appendCheckedItem( MENU_FILE_CHECK, _T( "&Check Menu Item" ), & MainWindow::menuClicked );
	itsMenu->attach( this );

	// Creating alternative menu...
	itsMenu2 = createMenu();
	file = itsMenu2->appendPopup( _T( "&Svada" ) );
	file->appendItem( MENU_FILE_CLEAR, _T( "&Clear" ), & MainWindow::menuClicked );

	// Manipulating the system menu
	itsMenuSys = itsMenu->getSystemMenu();
	itsMenuSys->appendSeparatorItem();
	itsMenuSys->appendItem( MENU_FILE_ABOUT, _T( "&About" ), & MainWindow::menuClicked );

	// Creating our toolbar
	WidgetToolbar::Seed seed;
	seed.style |= TBSTYLE_FLAT;
	itsToolBar = createToolbar( seed );
	itsToolBar->setButtonSize( 32, 32 );

	ImageListPtr imageList( new sw::ImageList( 32, 32, ILC_COLOR24 ) );
	imageList->addMultiple( sw::Bitmap( IDB_BITMAP_TOOLBAR ) );
	itsToolBar->setNormalImageList( imageList );

	ImageListPtr disabledImageList( new sw::ImageList( 32, 32, ILC_COLOR24 | ILC_MASK ) );
	disabledImageList->addMultiple( sw::Bitmap( IDB_BITMAPDISABLED ) );
	itsToolBar->setDisabledImageList( disabledImageList );

	ImageListPtr hotImageList( new sw::ImageList( 32, 32, ILC_COLOR24 | ILC_MASK ) );
	for ( int i = 0; i < 4; i++ )
	{
		hotImageList->add( sw::Icon( IDI_ICONHOT ) );
	}
	itsToolBar->setHotImageList( hotImageList );

	itsToolBar->addButton( 1, 0, _T( "Test Tooltip!" ), & MainWindow::toolbarClicked );
	itsToolBar->addButton( 2, 1, _T( "Test ** 2 ** Tooltip!" ), & MainWindow::toolbarClicked );
	itsToolBar->addButton( 3, 2, _T( "Test IV tooltip\r\nHowdy doodie woodie!" ), & MainWindow::toolbarClicked );
	itsToolBar->addButton( 4, 3, _T( "Final tooltip!" ), & MainWindow::toolbarClicked );

	// Creating status bar
	itsStatusBar = createStatusBar();
	itsStatusBar->onDblClicked( & MainWindow::statusBarDblClicked );

	// Creating the Static field...
	itsLblExp = createStatic();
	itsLblExp->setBounds( 10, 500, 300, 150 );
	itsLblExp->setText( _T( "Try to resize the window and press the button, and especially try to choose \"About\" from the \"Help\" menu... ;)" )
		_T( "\r\nOr try to click this text...\r\nOr even maybe double click the status bar..." ) );
	itsLblExp->onClicked( & MainWindow::staticTextClicked );

	// Creating Genetic Tree Explanation...
	itsLblGenTree = createStatic();
	itsLblGenTree->setBounds( 400, 350, 400, 75 );
	itsLblGenTree->setText( _T( "This WidgetTree actually shows a \"Genetic Tree\", " )
		_T( "for those interested the subject can be studied at " )
		_T( "http : //www.geocities.com/polterguy1000 \r\n(doubleclick THIS text)") );
	itsLblGenTree->onDblClicked( & MainWindow::staticTextGenTre );

	// Doing something which is even FUNNIER!!! ;)
	SmartWin::Command cmd( _T( "CheckAlarms" ) );
	createTimer( checkForAlarms, SCROLL_SPEED, cmd );
	onSized( & MainWindow::sized );
	onMoved( & MainWindow::moved );

	// Creating button
	itsButton = createButton();
	itsButton->setText( _T( "PUSH ME!" ) );
	itsButton->setBounds( 100, 350, 150, 100 );
	itsButton->onClicked( & MainWindow::btnClicked );

	// Creating text field
	itsTxtField = createRichTextBox();

	// Resizing to trigger resize event...
	setBounds( 10, 100, 1000, 800 );

	// Creating the dialog...
	itsDialogWidget = new ::DialogWidget( this );
}

MainWindow::~MainWindow()
{
}
