// $Revision: 1.30 $
/* Copyright (c) 2005 - Thomas Hansen
 * Part of the SmartWin++ library example code
 * Permission to use under the same terms as the actual library
 * The license to the library can be found at http://smartwinlib.org
 * License for library is Open Source and BSD
 */
#include "DataGrid.h"
#include "TabSheetWidget.h"
#include "resource.h"

using namespace SmartWin;

DataGrid::DataGrid( TabSheetWidget * parent )
		: Widget( parent )
		, sw::WidgetFactory< sw::WidgetChildWindow, DataGrid >( parent )
		, itsParent( parent )
{}

void DataGrid::layout()
{
	// Resizing main client area
	sw::Rectangle rc( itsParent->getClientAreaSize() );
	setBounds( 10, 30, rc.size.x - 20, rc.size.y - 40 );

	// Resizing static text widgets
	const sw::Rectangle rcClientArea( getClientAreaSize() );
	rc = rcClientArea.getTop( 16 );
	rc.pos.y += 2;
	itsStatName->setSizeAsRow( rc, 4, 0, 2 );
	itsStatEmail->setSizeAsRow( rc, 4, 1, 2 );
	itsStatNick->setSizeAsRow( rc, 4, 2, 2 );

	// Resizing textboxes and button
	rc = rcClientArea.getTop( 36 );
	rc.pos.y = 20;
	rc.size.y -= 20;
	itsName->setSizeAsRow( rc, 4, 0, 2 );
	itsEmail->setSizeAsRow( rc, 4, 1, 2 );
	itsNick->setSizeAsRow( rc, 4, 2, 2 );
	itsAddButton->setSizeAsRow( rc, 4, 3, 2 );

	// Resizing DataGrid
	rc = rcClientArea.shrink( 2, 38 );
	itsDataGrid->setBounds( rc );
	itsDataGrid->setColumnWidth( 0, ( int )( ( ( double ) rc.size.x / 100.0 ) * 25.0 ) );
	itsDataGrid->setColumnWidth( 1, ( int )( ( ( double ) rc.size.x / 100.0 ) * 45.0 ) );
	itsDataGrid->setColumnWidth( 2, ( int )( ( ( double ) rc.size.x / 100.0 ) * 30.0 ) );

	// Info static Widget
	rc = rcClientArea.getBottom( 36 );
	itsInfo->setBounds( rc );
}

void DataGrid::clicked( WidgetButtonPtr btn )
{
	// Retrieving data from Text Boxes and adding up a row to DataGrid
	SmartUtil::tstring name = itsName->getText();
	SmartUtil::tstring email = itsEmail->getText();
	SmartUtil::tstring nick = itsNick->getText();

	std::vector< SmartUtil::tstring > row;
	row.push_back( name );
	row.push_back( email );
	row.push_back( nick );
	itsDataGrid->insertRow( row );
}

void DataGrid::getFocus( WidgetTextBoxPtr txt )
{
	// On Windows CE based systems we must explicitly SHOW the Keyboard to be able to actually write stuff unless device have hardware keyboard...
#ifdef WINCE
	Application::instance().showSip( this, true );
#endif
}

void DataGrid::looseFocus( WidgetTextBoxPtr txt )
{
#ifdef WINCE
	Application::instance().showSip( this, false );
#endif
}

boost::shared_ptr< sw::Brush > DataGrid::backgroundColor( WidgetStaticPtr stat, sw::Canvas & canvas )
{
	canvas.setBkMode( true );
	itsBgBrush = boost::shared_ptr< sw::Brush >( new SmartWin::Brush( canvas, RGB( 128, 64, 128 ) ) );
	return itsBgBrush;
}

void DataGrid::columnClicked( WidgetDataGridPtr list, int colNo )
{
	if ( itsClickedColumn == colNo )
		itsShouldSortReverse = !itsShouldSortReverse;
	else
	{
		itsShouldSortReverse = false;
	}
	itsClickedColumn = colNo;
	list->sortList();
}

int DataGrid::sortCompare( WidgetDataGridPtr list, LPARAM left, LPARAM right )
{
	SmartUtil::tstring leftString = list->getCellTextByLParam( itsClickedColumn, left );
	SmartUtil::tstring rightString = list->getCellTextByLParam( itsClickedColumn, right );
	if ( leftString > rightString )
		return itsShouldSortReverse ? - 1 : 1;
	else if ( leftString < rightString )
		return itsShouldSortReverse ? 1 : - 1;
	else
		return 0;
}

void DataGrid::customDraw( WidgetDataGridPtr list, unsigned row, unsigned col, bool selected, Canvas & canvas, const sw::Rectangle & rc )
{
	SmartUtil::tstring txt = list->getCellTextByLParam( col, row );
	Brush( canvas, RGB( 255, 0, 0 ) );
	Brush tmpBrush( canvas, BitmapPtr( new Bitmap( selected ? IDB_BITMAP3 : IDB_BITMAP2 ) ) );
	canvas.fillRectangle( rc, tmpBrush );
	if ( selected )
		canvas.setTextColor( RGB( 198, 198, 255 ) );
	else
		canvas.setTextColor( RGB( 0, 128, 54 ) );
	if ( txt == _T( "" ) )
		return;
	canvas.drawText( txt, rc, DT_LEFT );
}

void DataGrid::selectionChanged( WidgetDataGridPtr list )
{
	list->updateWidget();
}

void DataGrid::listClicked( WidgetDataGridPtr list )
{
	itsInfo->setText( SmartUtil::tstring( _T( "DataGrid clicked, selection: " ) ) + list->getCellText( 0, list->getSelectedIndex() )
					  + _T( " item: " )
					  + boost::lexical_cast< SmartUtil::tstring >( list->getSelectedIndex() ) );
}

// Allthough this one doesn't really work (unless you double click OUTSIDE any existing items) because the internals will throw
// up an Edit Control in double click to enable changing of items/subitems it's still here for reference
// To see it in action try to double click OUTSIDE any ListView Items!!
void DataGrid::listDblClicked( WidgetDataGridPtr list )
{
	itsInfo->setText( SmartUtil::tstring( _T( "DataGrid DOUBLE clicked, selection: " ) ) + list->getCellText( 0, list->getSelectedIndex() )
		+ _T( " item: " )
		 + boost::lexical_cast< SmartUtil::tstring >( list->getSelectedIndex() ) );
}

void DataGrid::listRightClicked( WidgetDataGridPtr list )
{
	itsInfo->setText( SmartUtil::tstring( _T( "Tree RIGHT clicked, selection: " ) ) + list->getCellText( 0, list->getSelectedIndex() )
					  + _T( " item: " )
					  + boost::lexical_cast< SmartUtil::tstring >( list->getSelectedIndex() ) );

	// Only showing context sensitive menu if item is selected...
	if ( - 1 != list->getSelectedIndex() )
	{
		itsCtxMenu->trackPopupMenu( this );
	}
}

void DataGrid::menuEventHandler( WidgetMenuPtr menu, unsigned item )
{
	createMessageBox().show( menu->getText( item ) + SmartUtil::tstring( _T( " selected in grid: " ) )
							 + itsDataGrid->getCellText( 0, itsDataGrid->getSelectedIndex() ) );
}

void DataGrid::initAndCreate()
{
	// Creating main DataGrid container window
	createWindow();

	// Info static Widget
	itsInfo = createStatic();
	itsInfo->setText( _T( "Try to click items in DataGrid" ) );

	// Creating WidgetDataGrid (actual DataGrid)
	itsDataGrid = createDataGrid();
	std::vector< SmartUtil::tstring > cols;
	cols.push_back( _T( "Name" ) );
	cols.push_back( _T( "Email" ) );
	cols.push_back( _T( "Nickname" ) );
	itsDataGrid->createColumns( cols );
	itsDataGrid->setFullRowSelect();
	itsDataGrid->setGridLines();
	itsDataGrid->setHeaderDragDrop( true );
	itsDataGrid->onColumnHeaderClick( & DataGrid::columnClicked );
	itsDataGrid->onSortItems( & DataGrid::sortCompare );
	itsDataGrid->onCustomPainting( & DataGrid::customDraw );
	itsDataGrid->onSelectionChanged( & DataGrid::selectionChanged );
	itsDataGrid->onClicked( & DataGrid::listClicked );
	itsDataGrid->onDblClicked( & DataGrid::listDblClicked );
	itsDataGrid->onRightClicked( & DataGrid::listRightClicked );
	itsShouldSortReverse = false;
	itsClickedColumn = - 1;

	// Creating the font to display the DataGrid with
	FontPtr dataGridFont = createFont( _T( "Comic Sans" ), 16, 5, 1, ANSI_CHARSET, false, false, false, 0, OUT_DEFAULT_PRECIS );
	itsDataGrid->setFont( dataGridFont );

	// Populating DataGrid with some items
	std::vector< SmartUtil::tstring > row;
	row.push_back( _T( "Thomas" ) );
	row.push_back( _T( "polterguy@gmail.com" ) );
	row.push_back( _T( "polterguy" ) );
	itsDataGrid->insertRow( row );
	row.clear();
	row.push_back( _T( "Andrew" ) );
	row.push_back( _T( "andrew7webb@comcast.net" ) );
	row.push_back( _T( "andrew7webb" ) );
	itsDataGrid->insertRow( row );
	row.clear();
	row.push_back( _T( "Sergey" ) );
	row.push_back( _T( "sergey.hakobyan@viragelogic.com" ) );
	row.push_back( _T( "sergey" ) );
	itsDataGrid->insertRow( row );
	row.clear();
	row.push_back( _T( "Peter" ) );
	row.push_back( _T( "syntheticpp@gmx.net" ) );
	row.push_back( _T( "syntheticpp" ) );
	itsDataGrid->insertRow( row );

	// Creating the font to display the static text and the text boxes with
	FontPtr staticFont = createFont( _T( "Times New Roman" ), 14, 4, 1, ANSI_CHARSET, false, false, false, 0, OUT_DEFAULT_PRECIS );

	// Creating the textbox Widgets
	itsName = createTextBox();
	itsName->onFocus( & DataGrid::getFocus );
	itsName->onKillFocus( & DataGrid::looseFocus );
	itsName->setFont( staticFont );

	itsEmail = createTextBox();
	itsEmail->onFocus( & DataGrid::getFocus );
	itsEmail->onKillFocus( & DataGrid::looseFocus );
	itsEmail->setFont( staticFont );

	itsNick = createTextBox();
	itsNick->onFocus( & DataGrid::getFocus );
	itsNick->onKillFocus( & DataGrid::looseFocus );
	itsNick->setFont( staticFont );

	// Creating the static text widgets
	itsStatName = createStatic();
	itsStatName->setText( _T( "Name" ) );
	itsStatName->onBackgroundColor( & DataGrid::backgroundColor );
	itsStatName->setFont( staticFont );

	itsStatEmail = createStatic();
	itsStatEmail->setText( _T( "Email" ) );
	itsStatEmail->onBackgroundColor( & DataGrid::backgroundColor );
	itsStatEmail->setFont( staticFont );

	itsStatNick = createStatic();
	itsStatNick->setText( _T( "Nick" ) );
	itsStatNick->onBackgroundColor( & DataGrid::backgroundColor );
	itsStatNick->setFont( staticFont );

	// Creating the Add button
	itsAddButton = createButton();
	itsAddButton->setText( _T( "Add" ) );
	itsAddButton->onClicked( & DataGrid::clicked );

	itsMainMenu = createMenu();
	itsCtxMenu = itsMainMenu->appendPopup( _T( "File" ) );
	itsCtxMenu->appendItem( 0, _T( "Testing1" ), & DataGrid::menuEventHandler );
	itsCtxMenu->appendItem( 1, _T( "Testing2" ), & DataGrid::menuEventHandler );
	itsCtxMenu->appendItem( 2, _T( "Testing3" ), & DataGrid::menuEventHandler );

	// Once And Once Only
	layout();
}
