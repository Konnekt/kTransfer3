// $Revision: 1.22 $
// WidgetDataGrid sample program
//
#include "SmartWin.h"
#include <vector>
#include <assert.h>
#include "resource.h"
using namespace SmartWin;

// The "factory" version
class WidgetTest1
	: public WidgetFactory< WidgetWindow, WidgetTest1 >
{
	WidgetDataGridPtr list;
	WidgetDataGridPtr listCallback;
	WidgetMenuPtr mainMenu;
public:

	WidgetTest1()
	{}

	SmartWin::Rectangle doLayout( unsigned int a_Control )
	{
		static long d_Border = 15l;
		SmartWin::Rectangle d_Answer( getClientAreaSize() );

		d_Answer = d_Answer.shrink( d_Border );
		switch ( a_Control )
		{
		case 0 : // top list
			return d_Answer.top();
		case 1 : // bottom list
			d_Answer = d_Answer.bottom();
			d_Answer.size.y -= d_Border;
			d_Answer.pos.y += d_Border;
		}
		return d_Answer;
	}

	void sized( const SmartWin::WidgetSizedEventResult & newSize )
	{
		list->setBounds( doLayout( 0 ) );
		listCallback->setBounds( doLayout( 1 ) );
	}

	bool validateList( WidgetDataGridPtr list, unsigned int col, unsigned int row, SmartUtil::tstring & newValue )
	{
		return true;
	}

	void getItem( WidgetDataGridPtr list, LPARAM lPar, unsigned int col, unsigned int row, SmartUtil::tstring & insertionValue )
	{
		SmartUtil::tstring tmp2 = _T( "thomas" );

		insertionValue = boost::lexical_cast< SmartUtil::tstring >( row ) + _T( ":" )
			+ boost::lexical_cast< SmartUtil::tstring >( col ) + _T( " LPARAM: " ) + boost::lexical_cast< SmartUtil::tstring >( lPar );
	}

	void getIcon( WidgetDataGridPtr list, LPARAM lPar, unsigned int row, int & insertionValue )
	{
		// sets the icon; if you don't change the value, no icon will be shown
		insertionValue = row % 3;
	}

	void selectionChanged( WidgetDataGridPtr list )
	{
		int selected = list->getSelectedIndex();
	}

	int sortCompare( WidgetDataGridPtr list, LPARAM left, LPARAM right )
	{
		if ( left < right )
			return 1;
		else if ( right < left )
			return - 1;
		return 0;
	}

	bool closed()
	{
		return true;
	}

	void mouseMoved( WidgetDataGridPtr list, const MouseEventResult & res )
	{
		SmartUtil::tstring tmp = _T( "Mouse moved: " );
		tmp += boost::lexical_cast< SmartUtil::tstring >( res.pos.x );
		tmp += _T( ", " );
		tmp += boost::lexical_cast< SmartUtil::tstring >( res.pos.y );
		this->setText( tmp );
	}

	void mouseRightUp( WidgetDataGridPtr list, const MouseEventResult & res )
	{
		SmartUtil::tstring tmp = _T( "Mouse right UP: " );
		tmp += boost::lexical_cast< SmartUtil::tstring >( res.pos.x );
		tmp += _T( ", " );
		tmp += boost::lexical_cast< SmartUtil::tstring >( res.pos.y );
		this->setText( tmp );
	}

	void mouseLeftUp( WidgetDataGridPtr list, const MouseEventResult & res )
	{
		SmartUtil::tstring tmp = _T( "Mouse left UP: " );
		tmp += boost::lexical_cast< SmartUtil::tstring >( res.pos.x );
		tmp += _T( ", " );
		tmp += boost::lexical_cast< SmartUtil::tstring >( res.pos.y );
		this->setText( tmp );
	}

	void mouseMiddleUp( WidgetDataGridPtr list, const MouseEventResult & res )
	{
		SmartUtil::tstring tmp = _T( "Mouse middle UP: " );
		tmp += boost::lexical_cast< SmartUtil::tstring >( res.pos.x );
		tmp += _T( ", " );
		tmp += boost::lexical_cast< SmartUtil::tstring >( res.pos.y );
		this->setText( tmp );
	}

	void mouseRightDown( WidgetDataGridPtr list, const MouseEventResult & res )
	{
		SmartUtil::tstring tmp = _T( "Mouse right DOWN: " );
		tmp += boost::lexical_cast< SmartUtil::tstring >( res.pos.x );
		tmp += _T( ", " );
		tmp += boost::lexical_cast< SmartUtil::tstring >( res.pos.y );
		this->setText( tmp );

	listCallback->clearSelection();
	}

	void mouseLeftDown( WidgetDataGridPtr list, const MouseEventResult & res )
	{
		SmartUtil::tstring tmp = _T( "Mouse left DOWN: " );
		tmp += boost::lexical_cast< SmartUtil::tstring >( res.pos.x );
		tmp += _T( ", " );
		tmp += boost::lexical_cast< SmartUtil::tstring >( res.pos.y );
		this->setText( tmp );
	}

	void mouseMiddleDown( WidgetDataGridPtr list, const MouseEventResult & res )
	{
		SmartUtil::tstring tmp = _T( "Mouse middle DOWN: " );
		tmp += boost::lexical_cast< SmartUtil::tstring >( res.pos.x );
		tmp += _T( ", " );
		tmp += boost::lexical_cast< SmartUtil::tstring >( res.pos.y );
		this->setText( tmp );

		listCallback->setSelectedIndex( 0 );
	}

	void scrolled( WidgetDataGridPtr list )
	{
		this->setText( _T( "Scrolled" ) );
	}

  void menuEventHandler( WidgetMenuPtr menu, unsigned item )
	{
		// We're using the list view style as the menu item id!
		list->setView( item );
		listCallback->setView( item );
	}

	void initAndCreate()
	{
		using namespace SmartWin;

		WidgetWindow::Seed windowCS;

		// Seed has the advantage that the window is not relocated, thus reducing
		// flickering.
		windowCS.location = SmartWin::Rectangle( 100, 100, 450, 350 );
		windowCS.caption = _T( "WidgetDataGrid compile and execute test" );
		windowCS.background = CreateSolidBrush( RGB( 0, 0, 125 ) );
		createWindow( windowCS );

		onSized( & WidgetTest1::sized );
		onClosing( & WidgetTest1::closed );

		// Creating insertion list
		WidgetDataGrid::Seed seed;

		seed.exStyle = WS_EX_CLIENTEDGE;
		seed.location = doLayout( 0 );
		list = createDataGrid( seed );

		list->onMouseMove( & WidgetTest1::mouseMoved );

		//list->onLeftMouseDown( &WidgetTest1::mouseLeftDown );
		list->onRightMouseDown( & WidgetTest1::mouseRightDown );
		list->onMiddleMouseDown( & WidgetTest1::mouseMiddleDown );

		list->onLeftMouseUp( & WidgetTest1::mouseLeftUp );
		list->onRightMouseUp( & WidgetTest1::mouseRightUp );
		list->onMiddleMouseUp( & WidgetTest1::mouseMiddleUp );
		list->onScrollVert( & WidgetTest1::scrolled );

		// Creating small image list (16x16px, 32 bits, with mask)
		ImageListPtr smallImageList( new ImageList( 16, 16, ILC_COLOR32 | ILC_MASK ) );

		// Loading 3 bitmaps (1 mask) and 1 icon
		Bitmap bitmap1( IDB_BITMAP1 );
		Bitmap bitmap2( IDB_BITMAP2 );
		Bitmap mask( IDB_MASK );
		Icon icon1( IDI_ICON1 );

		// Adding images to the list
		smallImageList->add( bitmap1 ); //bitmap
		smallImageList->add( bitmap2, mask ); //bitmap with mask
		smallImageList->add( icon1 ); //icon (bultin mask)

		list->setSmallImageList( smallImageList );

		// Creating normal image list (32x32px, 32 bits, with mask)
		ImageListPtr imageList( new ImageList( 32, 32, ILC_COLOR32 | ILC_MASK ) );

		// Adding images to the list (must have the same number of elements of the small list)
		for ( int imgIdx = 0; imgIdx < 3; imgIdx++ )
		{
			//note, we're using the same icon for all the images, which is the same
			//icon used for the small list at index 2. Icons can have multiple
			//versions at different sizes in the same file, and the datagrid will choose
			//the most appropriate size to show.
			imageList->add( icon1 );
		}

		// Adding image list to datagrid
		list->setNormalImageList( imageList );

		// Creating callback list

		listCallback = createDataGrid( seed );
		listCallback->onGetItem( & WidgetTest1::getItem );
		listCallback->setAlwaysShowSelection();
		listCallback->onGetItem( & WidgetTest1::getItem );
		listCallback->onGetIcon( & WidgetTest1::getIcon );

		// Adding image lists (note, they're the same from the other list)
		listCallback->setSmallImageList( smallImageList );
		listCallback->setNormalImageList( imageList );

		std::vector< SmartUtil::tstring > columnNames;
		columnNames.push_back( _T( "One" ) );
		columnNames.push_back( _T( "Two" ) );
		columnNames.push_back( _T( "Three" ) );
		listCallback->createColumns( columnNames );
		for ( int idx = 0; idx < 1000; ++idx )
		{
			listCallback->insertCallbackRow( idx );
		}

		// Sorting list
		listCallback->onSortItems( & WidgetTest1::sortCompare );
		listCallback->sortList();
		listCallback->setReadOnly();

		// Inserting items into "insertion list"
		list->setFullRowSelect();
		list->setCheckBoxes();
		list->setGridLines();
		list->setHeaderDragDrop();
		list->setHoover();
		list->setSingleRowSelection();
		list->setReadOnly( true ); // Note! This one is overridden when given a "beenValidate" event handler, therefore it has no effect, included for "compile" time verifying...
		list->onValidate( & WidgetTest1::validateList );
		list->onSelectionChanged( & WidgetTest1::selectionChanged );

		// Creating columns
		std::vector< SmartUtil::tstring > columns;
		columns.push_back( _T( "Col1" ) );
		columns.push_back( _T( "Col2" ) );
		columns.push_back( _T( "Col3" ) );
		columns.push_back( _T( "Col4" ) );
		list->createColumns( columns );

		// Inserting values
		std::vector< SmartUtil::tstring > values;
		values.push_back( _T( "Row1Col1Value" ) );
		values.push_back( _T( "Row1Col2Value" ) );
		values.push_back( _T( "Row1Col3Value" ) );
		values.push_back( _T( "Row1Col4Value" ) );
		list->insertRow( values, - 1, - 1, 0 );

		values.clear();
		values.push_back( _T( "Row2Col1Value" ) );
		values.push_back( _T( "Row2Col2Value" ) );
		values.push_back( _T( "Row2Col3Value" ) );
		values.push_back( _T( "Row2Col4Value" ) );
		list->insertRow( values, - 1, - 1, 1 );

		mainMenu = createMenu();
		WidgetMenuPtr view = mainMenu->appendPopup( _T( "&View" ) );
		view->appendItem( LVS_REPORT, _T( "&Report" ), & WidgetTest1::menuEventHandler );
		view->appendItem( LVS_ICON, _T( "&Icon" ), & WidgetTest1::menuEventHandler );
		view->appendItem( LVS_SMALLICON, _T( "&Small Icon" ), & WidgetTest1::menuEventHandler );
		view->appendItem( LVS_LIST, _T( "&List" ), & WidgetTest1::menuEventHandler );
		mainMenu->attach( this );
	}
};

// Unit test for WidgetDataGrid
int SmartWinMain( Application & app )
{
	// Widget (Factory) version
	WidgetTest1 * testWnd1 = new WidgetTest1;
	testWnd1->initAndCreate();
	return app.run();
}
