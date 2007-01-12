// $Revision: 1.8 $
/* Copyright (c) 2005 - Thomas Hansen
 * Part of the SmartWin++ library example code
 * Permission to use under the same terms as the actual library
 * The license to the library can be found at http://smartwinlib.org
 * License for library is Open Source and BSD
 */
#ifndef mdi_grid_0587684_h
#define mdi_grid_0587684_h

#include "SmartWin.h"
using namespace SmartWin;
#include <string>
using namespace std;
#include "io/iolib.h"
#include "io/html_get.h"
#include "io/html_put.h"

/*
MdiGrid is a child of MdiFrame.

The MdiGrid class allows data to be created that it naturally arranged as
a series of named column vectors. The first column may be a row name.
No operations are defined upon the data except create, delete, and edit.
The data is string.

The data is stored in a strict .html file with one table.  All table entries
are of the format: "<td> data </td>" for easy parsing.

*/

template< class parent, class MdiParent >
class MdiGrid
			: public SmartWin::WidgetFactory< SmartWin::WidgetMDIChild, MdiGrid< parent, MdiParent >, SmartWin::MessageMapPolicyMDIChildWidget >
{
public:

private:
	typedef SmartWin::WidgetFactory< SmartWin::WidgetMDIChild, MdiGrid< parent, MdiParent >, SmartWin::MessageMapPolicyMDIChildWidget > FactoryType;
	SmartUtil::tstring itsCurrentFilename;
	typename MdiGrid< parent, MdiParent >::WidgetDataGridPtr itsGrid;
	vector< SmartUtil::tstring > itsColnames;
	typename MdiGrid< parent, MdiParent >::WidgetButtonPtr itsMenuBut;
	typename MdiGrid< parent, MdiParent >::WidgetStaticPtr itsPrompt;
	typename MdiGrid< parent, MdiParent >::WidgetTextBoxPtr itsInputBox;
	int itsBase;

	sw::FontPtr defaultfont();
	iolib itsIo;

	typename MdiGrid< parent, MdiParent >::WidgetMenuPtr itsMenu;
	typename MdiGrid< parent, MdiParent >::WidgetMenuPtr itsPopmenu;

	parent * itsParent;
	void ( parent::* m_close_callback_func ) ( MdiGrid * me );

	typedef enum MENU_ITEM
	{ OPEN = 1, SAVE, SAVE_AS, PRINT, EMAIL, FONT,
							 ADD_ROW, DEL_ROW, ADD_COL, DEL_COL,
							 CAN_TABLE
						   };

	typedef enum input_mode_type
	{
		NO_INPUT = 1, COL_NAME, COL_DATA, ROW_DATA
	};
	input_mode_type itsInputMode;
	unsigned int itsRow, itsCol;

	void menuClicked( typename MdiGrid::WidgetMenuPtr menu, unsigned item )
	{
		switch ( item - itsBase )
		{
			case OPEN : FileOpen(); break;
			case SAVE : FileSaveCurrentFile(); break;
			case SAVE_AS : FileSaveAs(); break;

			case EMAIL :
				{
					// string emailtext = string( textField->getText() );
					// itsIo.FileEmailFile( emailtext );
				}
				break;

			case PRINT :
				{
					// string printtext = string( textField->getText() );
					// itsIo.print( printtext );
				}
				break;

			case FONT : choose_font(); break;

			case ADD_ROW : add_row(); break;
			case DEL_ROW : del_row(); break;
			case ADD_COL : add_col_menu(); break;
			case DEL_COL : del_col(); break;

			case CAN_TABLE : canned_table(); break;
		}
	}

	void init_menu( int base )
	{
		itsBase = base;

		itsMenu = this->createMenu();
		itsPopmenu = itsMenu->appendPopup( _T( "&Menu" ) );

		typename MdiGrid< parent, MdiParent >::WidgetMenuPtr file = itsPopmenu->appendPopup( _T( "&File" ) );

		file->appendItem( OPEN + base, _T( "&Open" ), & MdiGrid< parent, MdiParent >::menuClicked );
		file->appendItem( SAVE + base, _T( "&Save" ), & MdiGrid< parent, MdiParent >::menuClicked );
		file->appendItem( SAVE_AS + base, _T( "Save &As" ), & MdiGrid< parent, MdiParent >::menuClicked );
		file->appendSeparatorItem();
		file->appendItem( PRINT + base, _T( "&Print" ), & MdiGrid< parent, MdiParent >::menuClicked );
		file->appendItem( EMAIL + base, _T( "&Email" ), & MdiGrid< parent, MdiParent >::menuClicked );
		file->appendItem( FONT + base, _T( "&Font" ), & MdiGrid< parent, MdiParent >::menuClicked );
		file->appendItem( CAN_TABLE + base, _T( "&Canned table" ), & MdiGrid< parent, MdiParent >::menuClicked );

		typename MdiGrid< parent, MdiParent >::WidgetMenuPtr rows = itsPopmenu->appendPopup( _T( "&Rows" ) );
		rows->appendItem( ADD_ROW + base, _T( "&add" ), & MdiGrid< parent, MdiParent >::menuClicked );
		rows->appendItem( DEL_ROW + base, _T( "&delete selected" ), & MdiGrid< parent, MdiParent >::menuClicked );

		typename MdiGrid< parent, MdiParent >::WidgetMenuPtr cols = itsPopmenu->appendPopup( _T( "&Columns" ) );
		cols->appendItem( ADD_COL + base, _T( "&add" ), & MdiGrid< parent, MdiParent >::menuClicked );
		cols->appendItem( DEL_COL + base, _T( "&delete last column" ), & MdiGrid< parent, MdiParent >::menuClicked );
	}

	void PopupMenuViaRightClick( const SmartWin::MouseEventResult & mouse )
	{
		SmartWin::Rectangle possize = this->getBounds();
		int x = possize.pos.x + mouse.pos.x;
		int y = possize.pos.y + mouse.pos.y;

		::TrackPopupMenu( reinterpret_cast< HMENU >( itsPopmenu->handle() ),
						  TPM_CENTERALIGN | TPM_VCENTERALIGN,
						  x, y, 0, this->handle(), 0 );
	}

	void PopupMenuViaButton( typename MdiGrid::WidgetButtonPtr btn )
	{
		SmartWin::Rectangle btn_possize = btn->getBounds();

		itsPopmenu->trackPopupMenu( this, btn_possize.pos.x, btn_possize.pos.y + btn_possize.size.y, TPM_LEFTALIGN | TPM_TOPALIGN );
	}

	void HtmlFilenameFromUser( SmartUtil::tstring & txtfilename )
	{
		typename MdiGrid< parent, MdiParent >::WidgetLoadFile dlg = this->createLoadFile();
		dlg.addFilter( _T( "Text files (*.html)" ), _T( "*.html" ) );
		dlg.addFilter( _T( "All files (*.*)" ), _T( "*.*" ) );

		txtfilename = dlg.showDialog();
	}

	/*
	<html>
	<head>
	<title> </title>
	</head>
	<body>

	<table>
	<tr>
	<TH> Name </TH>
	<TH> Phone </TH>
	</tr>

	<tr>
	<td> Andrew </td>
	<td> 43 </td>
	</tr>

	<tr>
	<td> Fred </td>
	<td> 54 </td>
	</tr>

	</TABLE>
	</body>
	</html>

	*/
	// <tr> <td> data1 </td> <td> data1 </td> </tr>
	//
	bool parseRow( stringstream & table, vector< string > & vals )
	{
		string tag, data, token, endtag;
		table >> tag;
		if ( tag != "<tr>" ) return ( false );

		while ( true )
		{
			table >> tag;
			if ( ( tag != "<td>" ) && ( tag != "<th>" ) )
			{
				return ( tag == "</tr>" );
			}

			table >> data;
			if ( data == "</td>" )
			{
				data = "";
			}
			else
			{
				table >> endtag;
				while ( ( endtag != "</td>" ) && ( endtag != "</th>" ) )
				{
					data += _T( " " );
					data += endtag;
					table >> endtag;
					if ( 0 == endtag.size() ) return false;
				}
			}
			vals.push_back( data );
		}
	}

	void FileOpen()
	{
		SmartUtil::tstring filePath;
		HtmlFilenameFromUser( filePath );

		fstream file( filePath.c_str(), ios_base::in );
		if ( ! file.good() ) return;
		itsCurrentFilename = filePath;
		this->setText( itsCurrentFilename + _T( " - Grid" ) );

		bool ok;
		html_get hg( file, ok );
		if ( ! ok ) return;

		if ( ! hg.get( itsGrid ) )
		{
			this->createMessageBox().show( _T( "Table parsing error." ), _T( "HTML parse error" ) );
		}
	}

	void FileSaveAs()
	{
		typename MdiGrid::WidgetSaveFile dlg = this->createSaveFile();
		dlg.addFilter( _T( "HTML table files (*.html)" ), _T( "*.html" ) );
		dlg.addFilter( _T( "All Files (*.*)" ), _T( "*.*" ) );
		SmartUtil::tstring filePath = dlg.showDialog();
		itsCurrentFilename = filePath;
		this->setText( itsCurrentFilename + _T( " - Grid" ) );

		FileSave( filePath );
	}

	void FileSaveCurrentFile()
	{
		if ( itsCurrentFilename.size() > 0 )
		{
			FileSave( itsCurrentFilename );
		}
	}

	void FileSave( SmartUtil::tstring & filePath )
	{
		fstream file( filePath.c_str(), ios_base::out );
		if ( ! file.good() ) return;

		html_put hp( file );
		hp.put( itsGrid );
	}

	void init_font()
	{
		sw::FontPtr font = createFont( _T( "Times New Roman" ), 20, 10, 1,
											 ANSI_CHARSET, false, false, false, 0, OUT_DEFAULT_PRECIS );

		itsGrid->setFont( font );
	}

	void choose_font()
	{
		CHOOSEFONT cf;
		LOGFONT lf; // logical font structure
		DWORD rgbCurrent = 0; // current text color

		// Initialize CHOOSEFONT
		ZeroMemory( & cf, sizeof( CHOOSEFONT ) );
		cf.lStructSize = sizeof( CHOOSEFONT );
		cf.hwndOwner = this->handle();
		cf.lpLogFont = & lf;
		cf.rgbColors = rgbCurrent;
		cf.Flags = CF_SCREENFONTS | CF_EFFECTS;

		if ( ChooseFont( & cf ) )
		{
			sw::FontPtr font = SmartWin::createFont( lf.lfFaceName, lf.lfHeight, lf.lfWidth, lf.lfWeight,
												 lf.lfCharSet,
												 TRUE == lf.lfItalic, TRUE == lf.lfUnderline, TRUE == lf.lfStrikeOut );

			itsGrid->setFont( font );
		}
	}

	bool closeMdi()
	{
		// Invoke the callback function.

		( itsParent->*m_close_callback_func ) ( this );
		return true;
	}

	void isResized( const SmartWin::WidgetSizedEventResult & sz )
	{
		layout( sz.newSize );
	}

	void add_row()
	{
		if ( itsGrid->getColumnCount() > 0 )
		{
			itsGrid->insertRow(); // Empty row
			itsInputMode = ROW_DATA;
			itsCol = 0;
			RowDataPrompt();
			itsInputBox->setFocus();
		}
		else
		{
			this->createMessageBox().show( _T( "Need to define your columns first." ), _T( "Row can't be added yet" ) );
		}
	}

	void RowDataPrompt()
	{
		SmartUtil::tstring promptmsg;
		promptmsg = _T( "Row data for " );
		promptmsg += itsColnames[ itsCol ];
		promptmsg += _T( ":" );

		itsPrompt->setText( promptmsg );
	}

	void del_row()
	{
		int sel = itsGrid->getSelectedIndex();
		if ( - 1 == sel ) return;

		itsGrid->removeRow( sel );
		itsInputMode = NO_INPUT;
	}

	void add_col_menu()
	{
		itsPrompt->setText( "Add column named : " );
		itsInputMode = COL_NAME;
		itsInputBox->setFocus();
	}

	void add_col( const string & column_name )
	{
		vector< vector< string > > data;
		vector< string > row;
		int colcount = ( int ) itsGrid->getColumnCount();
		int rowcount = ( int ) itsGrid->getRowCount();

		// Save data since createColumns() erases the data.
		for ( int r = 0; r < rowcount; r++ )
		{
			data.push_back( row );
			for ( int c = 0; c < colcount; c++ )
			{
				data[ r ].push_back( itsGrid->getCellText( c, r ) );
			}
			data[ r ].push_back( "" ); // Extra unused column
		}

		itsColnames.push_back( column_name ); colcount++;
		itsGrid->createColumns( itsColnames );

		RedoColumnWidths( colcount );

		// Restore the data to the table.
		for ( int r = 0; r < rowcount; r++ )
		{
			itsGrid->insertRow( data[ r ] );
		}
	}

	void RedoColumnWidths( int colcount )
	{
		if ( 0 == colcount ) return;

		int colwidth = itsGrid->getSize().x / colcount;
		for ( int c = 0; c < colcount; c++ )
		{
			itsGrid->setColumnWidth( c, colwidth ); // or LVSCW_AUTOSIZE_USEHEADER
		}
	}

	void del_col()
	{
		unsigned int last_col = itsGrid->getColumnCount() - 1;
		if ( last_col > 0 )
		{
			itsGrid->deleteColumn( last_col );
			itsColnames.pop_back();
		}
	}

	void getItem( typename MdiGrid::WidgetDataGridPtr list, LPARAM lPar,
				  unsigned int col, unsigned int row,
				  SmartUtil::tstring & insertionValue )
	{
		basic_stringstream< TCHAR > stream;
		stream << row << " " << col;
		insertionValue = stream.str();
	}

	bool validate( typename MdiGrid::WidgetDataGridPtr list, unsigned int col, unsigned int row,
				   SmartUtil::tstring & newValue )
	{
		return ( true );
	}

	void OnTextReady( const string & newtext )
	{
		switch ( itsInputMode )
		{
			case NO_INPUT :
				break;

			case COL_NAME :
				add_col( newtext );
				if ( itsGrid->getRowCount() > 0 )
				{
					itsInputMode = COL_DATA;
					itsPrompt->setText( "Column data: " );
					itsRow = 0;
				}
				break;

			case COL_DATA :
				itsGrid->setCellText( itsGrid->getColumnCount() - 1, itsRow++, newtext );
				if ( itsRow >= itsGrid->getRowCount() )
				{
					itsInputMode = NO_INPUT;
					itsPrompt->setText( "" );
					itsGrid->setFocus();
				}
				break;

			case ROW_DATA :
				itsGrid->setCellText( itsCol++, itsGrid->getRowCount() - 1, newtext );
				if ( itsCol >= itsGrid->getColumnCount() )
				{
					itsInputMode = NO_INPUT;
					itsPrompt->setText( "" );
					itsGrid->setFocus();
				}
				else
				{
					RowDataPrompt();
				}
				break;
		}
	}

	bool KeyPressed( typename MdiGrid::WidgetTextBoxPtr textField, int key )
	{
		switch ( key )
		{
			case VK_RETURN :
				OnTextReady( itsInputBox->getText() );
				itsInputBox->setText( "" );
				return true;

			case VK_ESCAPE :
				itsPrompt->setText( "" );
				itsInputBox->setText( "" );
				itsInputMode = NO_INPUT;
				return true;
			default: return false;
		}

		return true;
	}

	void sim_entry( char txt[] )
	{
		itsInputBox->setText( txt ); KeyPressed( itsInputBox, VK_RETURN );
	}

	void canned_table()
	{
		add_col( string( "Year" ) );
		add_col( string( "Sales" ) );
		add_col( string( "Profits" ) );
		add_row();
		sim_entry( "2000" ); sim_entry( "1020" ); sim_entry( "210" );

		add_row();
		sim_entry( "2001" ); sim_entry( "1120" ); sim_entry( "250" );

		add_row();
		sim_entry( "2002" ); sim_entry( "1280" ); sim_entry( "290" );

		itsGrid->setSelectedIndex( 1 );
		del_row();

		add_col_menu();
		sim_entry( "Dividends" );
		sim_entry( "75" );
		sim_entry( "77" );
	}

	/*
	IN:
	DOES:   Uses 
	OUT:    rc is set to client relative coordinates 0,0  sizex, sizey
	*/
	void layout( SmartWin::Point newsize )
	{
		SmartWin::Rectangle rc( newsize );

		const int menu_size_y = 24;
		const int menu_size_x = 60;
		const int prompt_size_x = 150;
		const int menprom_size_x = menu_size_x + prompt_size_x;
		const int input_size_x = rc.size.x - menprom_size_x;

		rc.size.y -= menu_size_y; // Allocate space for menu
		itsGrid->setBounds( rc );
		RedoColumnWidths( itsGrid->getColumnCount() );

		itsMenuBut->setBounds( 0, rc.size.y, menu_size_x, menu_size_y );
		itsPrompt->setBounds( menu_size_x, rc.size.y, prompt_size_x, menu_size_y );
		itsInputBox->setBounds( menprom_size_x, rc.size.y, input_size_x, menu_size_y );
	}

public:
	MdiGrid( parent * myparent, MdiParent * mdiParent,
			  void ( parent::* callbackfunc ) ( MdiGrid * me ),
			  int base
			)
			: Widget( mdiParent ),
			itsParent( myparent ),
			m_close_callback_func( callbackfunc )
	{
		string trace;

		typename FactoryType::Seed seedMdiChild;
		seedMdiChild.style = WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_OVERLAPPEDWINDOW;
		this->createMDIChild( seedMdiChild );

		// The menu button
		itsMenuBut = this->createButton();
		itsMenuBut->setText( "menu" );
		itsMenuBut->onClicked( & MdiGrid::PopupMenuViaButton );

		// The prompt for element input.
		typename FactoryType::WidgetStatic::Seed seedStatic;
		seedStatic.style = WS_VISIBLE | ES_RIGHT;
		itsPrompt = this->createStatic( seedStatic );

		// The input text box.
		typename FactoryType::WidgetTextBox::Seed seedText;
		seedText.style = WS_VISIBLE | ES_WANTRETURN | ES_MULTILINE;
		itsInputBox = this->createTextBox( seedText );
		itsInputBox->onKeyPressed( & MdiGrid< parent, MdiParent >::KeyPressed );

		// The grid (WidgetDataGrid)
		// See documentation for Edit Control Styles from CreateWindow().
		typename FactoryType::WidgetDataGrid::Seed seedGrid;
		seedGrid.style = WS_VISIBLE | LVS_REPORT | LVS_EDITLABELS | LVS_SHOWSELALWAYS | WS_HSCROLL | WS_VSCROLL;
		seedGrid.exStyle = 0;

		itsGrid = this->createDataGrid( seedGrid );
		layout( seedGrid.location.size ); // All Widgets must be created already.

		itsGrid->createColumns( itsColnames );
		itsGrid->setGridLines();
		itsGrid->setHeaderDragDrop();
		itsGrid->setFullRowSelect();

		itsGrid->onGetItem( & MdiGrid< parent, MdiParent >::getItem );
		itsGrid->onValidate( & MdiGrid< parent, MdiParent >::validate );

		// Fonts and menus
		init_font();
		init_menu( base );

		// Handlers
		onSized( & MdiGrid< parent, MdiParent >::isResized );
		onClosing( & MdiGrid< parent, MdiParent >::closeMdi );
		onRightMouseDown( & MdiGrid< parent, MdiParent >::PopupMenuViaRightClick );
	}

	~MdiGrid()
	{}
};

#endif
