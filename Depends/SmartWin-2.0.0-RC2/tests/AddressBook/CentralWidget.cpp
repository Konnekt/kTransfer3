// $Revision: 1.16 $
#include "CentralWidget.h"
#include "MainWindow.h"
#include "TabWidget.h"
#include <fstream>

ABCentralWidget::ABCentralWidget( ABMainWindow * parent )
		: Widget( parent )
{
	// Also to add a child Widget we need to explicitly say so!
	createWindow( WidgetChildWindow::getDefaultSeed() );

	// Also we need to accomodate for the size "by hand" which of course gives us more flexibility though...
	sw::Rectangle rc( parent->getClientAreaSize() );
	setBounds( 0, 0, rc.size.x, rc.size.y );

	setupTabWidget();
	setupListView();
	onSized( & ABCentralWidget::sized );
}

void ABCentralWidget::save( const SmartUtil::tstring & path )
{
	std::ofstream file( path.c_str() );
	for ( unsigned idx = 0; idx < itsListView->getRowCount(); ++idx )
	{
		file << itsListView->getCellText( 0, idx ) << std::endl;
		file << itsListView->getCellText( 1, idx ) << std::endl;
		file << itsListView->getCellText( 2, idx ) << std::endl;
		file << itsListView->getCellText( 3, idx ) << std::endl;
	}
}

void ABCentralWidget::load( const SmartUtil::tstring & path )
{
	// BIG difference!
	// While Qt uses it's own classes for things like files etc, SmartWin++ uses the standard classes where ever possible!
	std::ifstream file( path.c_str() );
	while ( true )
	{
		std::string firstName, lastName, address, email;
		std::getline( file, firstName );
		std::getline( file, lastName );
		std::getline( file, address );
		std::getline( file, email );

		// Checking to see if we got ALL data
		if ( !file.good() || file.eof() )
			break;

		add( firstName, lastName, address, email );
	}
}

/* Copyright (c) 2005 - Thomas Hansen
 * Part of the SmartWin++ library example code
 * Permission to use under the same terms as the actual library
 * The license to the library can be found at http://smartwinlib.org
 * License for library is Open Source and BSD
 */
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
		std::vector< SmartUtil::tstring > row; // Yet another example of where SmartWin++ uses standardized container classes
		row.push_back( firstName );
		row.push_back( lastName );
		row.push_back( address );
		row.push_back( email );
		itsListView->insertRow( row );
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
	for ( int idx = itsListView->getSelectedIndex() + 1; idx < ( int ) itsListView->getRowCount(); ++idx )
	{
		if ( idx < 0 )
			idx = 0; // NO selected item
		if ( firstName.size() > 0 && firstName == itsListView->getCellText( 0, idx ) )
			found = true;
		else if ( firstName.size() > 0 )
			found = false;
		if ( found && lastName.size() > 0 && lastName == itsListView->getCellText( 1, idx ) )
			found = true;
		else if ( lastName.size() > 0 )
			found = false;
		if ( found && address.size() > 0 && address == itsListView->getCellText( 2, idx ) )
			found = true;
		else if ( address.size() > 0 )
			found = false;
		if ( found && email.size() > 0 && email == itsListView->getCellText( 3, idx ) )
			found = true;
		else if ( email.size() > 0 )
			found = false;

		if ( found )
		{
			itsListView->setSelectedIndex( idx );
			itsListView->setFocus();
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
	itsTabWidget = new ABTabWidget( this );
}

void ABCentralWidget::setupListView()
{
	// Trying to create a cool looking ListView Widget
	WidgetDataGrid::Seed dataGridCS;

	dataGridCS.style |= WS_BORDER | WS_THICKFRAME | LVS_REPORT | LVS_EDITLABELS | LVS_SHOWSELALWAYS;
	itsListView = createDataGrid( dataGridCS );
	std::vector< SmartUtil::tstring > cols;
	cols.push_back( _T( "First Name" ) );
	cols.push_back( _T( "Last Name" ) );
	cols.push_back( _T( "Address" ) );
	cols.push_back( _T( "Email" ) );
	itsListView->createColumns( cols );
	itsListView->setFullRowSelect();
	itsListView->setGridLines();
	itsListView->setHeaderDragDrop();
	itsListView->setHoover();
	itsListView->setSingleRowSelection();
}

void ABCentralWidget::sized( const sw::WidgetSizedEventResult & sz )
{
	// Also we need to accomodate for the size "by hand" which of course gives us more flexibility though...
	sw::Rectangle rc( getClientAreaSize() );

	// Resizing tab control
	itsTabWidget->setBounds( 10, 10, rc.size.x - 20, 100 );
	itsListView->setBounds( 10, 120, rc.size.x - 20, rc.size.y - 130 );
}
