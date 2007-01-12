// $Revision: 1.10 $
#include "CentralWidget.h"
#include "MainWindow.h"
#include "TabWidget.h"
#include <fstream>

ABCentralWidget::ABCentralWidget( ABMainWindow * parent )
	: Widget( parent )
{
	// Initializing!
	createWindow();
	SmartWin::Rectangle rc = sw::Rectangle( parent->getClientAreaSize() );

	// Initializing tab and list...
	setupTabWidget();
	setupListView();

	// Setting onSized event handler and resizing (Once And Once Only...!)
	onSized( & ABCentralWidget::sized );
	setBounds( 0, 0, rc.size.x, rc.size.y );
}

void ABCentralWidget::save( const SmartUtil::tstring & path )
{
	std::basic_ofstream< TCHAR > file( path.c_str() );
	for ( unsigned idx = 0; idx < listView->getRowCount(); ++idx )
	{
		// Should probably supprot UNICODE here, but then I wouldn't get to demonstrate the nice conversion classes... ;)
		file << listView->getCellText( 0, idx ) << std::endl;
		file << listView->getCellText( 1, idx ) << std::endl;
		file << listView->getCellText( 2, idx ) << std::endl;
		file << listView->getCellText( 3, idx ) << std::endl;
	}
}

void ABCentralWidget::load( const SmartUtil::tstring & path )
{
	using namespace SmartWin;
	using namespace SmartUtil;
	std::basic_ifstream< TCHAR > file( path.c_str() );
	while ( true )
	{
		tstring firstName, lastName, address, email;
		std::getline( file, firstName );
		std::getline( file, lastName );
		std::getline( file, address );
		std::getline( file, email );

		// Checking to see if we got ALL data
		if ( !file.good() || file.eof() )
			break;

		// Should probably supprot UNICODE here, but then I wouldn't get to demonstrate the nice conversion classes... ;)
		add(
			firstName,
			lastName,
			address,
			email );
	}
}

void ABCentralWidget::add( const SmartUtil::tstring & firstName, const SmartUtil::tstring & lastName,
						  const SmartUtil::tstring & address, const SmartUtil::tstring & email )
{
	bool valid = false;
	if ( firstName.size() != 0 || lastName.size() != 0 || address.size() != 0 || email.size() != 0 )
	{
		if ( firstName.find_first_not_of( _T( " \t\r\n" ) ) != std::string::npos
			|| firstName.find_first_not_of( _T( " \t\r\n" ) ) != std::string::npos
			|| firstName.find_first_not_of( _T( " \t\r\n" ) ) != std::string::npos
			|| firstName.find_first_not_of( _T( " \t\r\n" ) ) != std::string::npos )
		{
			valid = true;
		}
	}
	if ( valid )
	{
		std::vector< SmartUtil::tstring > row;
		row.push_back( firstName );
		row.push_back( lastName );
		row.push_back( address );
		row.push_back( email );
		listView->insertRow( row );
	}
	else
	{
		createMessageBox().show( _T( "Can't have an \"empty\" address record" ), _T( "ERROR!" ),
			WidgetMessageBox::BOX_OK, WidgetMessageBox::BOX_ICONSTOP );
	}
}

void ABCentralWidget::search( const SmartUtil::tstring & firstName, const SmartUtil::tstring & lastName,
						  const SmartUtil::tstring & address, const SmartUtil::tstring & email )
{
	bool found = false;
	for ( int idx = listView->getSelectedIndex() + 1; idx < ( int ) listView->getRowCount(); ++idx )
	{
		if ( idx < 0 )
			idx = 0; // NO selected item
		if ( firstName.size() > 0 && firstName == listView->getCellText( 0, idx ) )
			found = true;
		else if ( firstName.size() > 0 )
			found = false;
		if ( found && lastName.size() > 0 && lastName == listView->getCellText( 1, idx ) )
			found = true;
		else if ( lastName.size() > 0 )
			found = false;
		if ( found && address.size() > 0 && address == listView->getCellText( 2, idx ) )
			found = true;
		else if ( address.size() > 0 )
			found = false;
		if ( found && email.size() > 0 && email == listView->getCellText( 3, idx ) )
			found = true;
		else if ( email.size() > 0 )
			found = false;

		if ( found )
		{
			listView->setSelectedIndex( idx );
			listView->setFocus();
			break;
		}
	}
	if ( !found )
	{
		createMessageBox().show( _T( "Not found" ) );
	}
}

void ABCentralWidget::setupTabWidget()
{
	tabWidget = new ABTabWidget( this );
}

void ABCentralWidget::setupListView()
{
	// Trying to create a cool looking ListView Widget
	listView = createDataGrid();
	std::vector< SmartUtil::tstring > cols;
	cols.push_back( _T( "Firstname" ) );
	cols.push_back( _T( "Sirname" ) );
	cols.push_back( _T( "Addr." ) );
	cols.push_back( _T( "Email" ) );
	listView->createColumns( cols );
	listView->setFullRowSelect();
	listView->setGridLines();
	listView->setHeaderDragDrop();
	listView->setSingleRowSelection();
	listView->setColumnWidth( 0, LVSCW_AUTOSIZE_USEHEADER );
	listView->setColumnWidth( 1, LVSCW_AUTOSIZE_USEHEADER );
	listView->setColumnWidth( 2, LVSCW_AUTOSIZE_USEHEADER );
	listView->setColumnWidth( 3, LVSCW_AUTOSIZE_USEHEADER );
}

void ABCentralWidget::sized( const SmartWin::WidgetSizedEventResult & sz )
{
	// Also we need to accomodate for the size "by hand" which off course gives us more flexibility though...
	SmartWin::Rectangle rc = sw::Rectangle( getClientAreaSize() );

	// Resizing tab control
	tabWidget->setBounds( 2, 27, rc.size.x - 4, 100 );
	listView->setBounds( 2, 127, rc.size.x - 4, rc.size.y - 132 );
}
