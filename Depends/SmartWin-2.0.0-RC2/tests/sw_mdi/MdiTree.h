// $Revision: 1.8 $
/* Copyright (c) 2005 - Thomas Hansen
 * Part of the SmartWin++ library example code
 * Permission to use under the same terms as the actual library
 * The license to the library can be found at http://smartwinlib.org
 * License for library is Open Source and BSD
 */
#ifndef mdi_tree_9847362_h
#define mdi_tree_9847362_h

#include "SmartWin.h"
using namespace SmartWin;
#include <string>
using namespace std;
#include "io/iolib.h"
#include "io/html_get.h"
#include "io/html_put.h"
/*
MdiTree is a child of MdiFrame.

The MdiTree class allows data to be created in a multi-tree with
multiple roots.

The data is stored in a strict .html file with nested unordered lists.
For example:
<ul>

<li> node1 </li>

<li> node2
	<ul>
	<li> node21 </li>
	<li> node22 </li>
	</ul>
</li>

</ul>

*/

template< class parent, class MdiParent >
class MdiTree
			: public SmartWin::WidgetFactory< SmartWin::WidgetMDIChild, MdiTree< parent, MdiParent >, SmartWin::MessageMapPolicyMDIChildWidget >
{
public:

private:
	typedef SmartWin::WidgetFactory< SmartWin::WidgetMDIChild, MdiTree< parent, MdiParent >, SmartWin::MessageMapPolicyMDIChildWidget > FactoryType;
	SmartUtil::tstring itsCurrentFilename;
	typename MdiTree< parent, MdiParent >::WidgetTreeViewPtr itsTree;
	vector< SmartUtil::tstring > itsColnames;
	typename MdiTree< parent, MdiParent >::WidgetButtonPtr itsMenuBut, itsAddBut;
	typename MdiTree< parent, MdiParent >::WidgetTextBoxPtr itsInputBox;
	int itsBase;
	sw::FontPtr defaultfont();
	iolib itsIo;
	typename MdiTree< parent, MdiParent >::WidgetMenuPtr itsMenu;
	typename MdiTree< parent, MdiParent >::WidgetMenuPtr itsPopmenu;
	parent * itsParent;
	void ( parent::* m_close_callback_func ) ( MdiTree * me );

	typedef enum MENU_ITEM
	{ OPEN = 1, SAVE, SAVE_AS, PRINT, EMAIL, FONT,
							 ADD_ROW, DEL_ROW, ADD_COL, DEL_COL,
							 CAN_TABLE
						   };

	void menuClicked( typename MdiTree< parent, MdiParent >::WidgetMenuPtr menu, unsigned item )
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

				/*
						case ADD_ROW:           add_row();      break;
						case DEL_ROW:           del_row();      break;
						case ADD_COL:           add_col_menu(); break;
						case DEL_COL:           del_col();      break;

						case CAN_TABLE:         canned_table(); break;
				*/
		}
	}

	void init_menu( int base )
	{
		itsBase = base;

		itsMenu = this->createMenu();
		itsPopmenu = itsMenu->appendPopup( _T( "&Menu" ) );

		typename MdiTree< parent, MdiParent >::WidgetMenuPtr file = itsPopmenu->appendPopup( _T( "&File" ) );

		file->appendItem( OPEN + base, _T( "&Open" ), & MdiTree:: menuClicked );
		file->appendItem( SAVE + base, _T( "&Save" ), & MdiTree::menuClicked );
		file->appendItem( SAVE_AS + base, _T( "Save &As" ), & MdiTree::menuClicked );
		file->appendSeparatorItem();
		file->appendItem( PRINT + base, _T( "&Print" ), & MdiTree::menuClicked );
		file->appendItem( EMAIL + base, _T( "&Email" ), & MdiTree::menuClicked );
		file->appendItem( FONT + base, _T( "&Font" ), & MdiTree::menuClicked );
		file->appendItem( CAN_TABLE + base, _T( "&Canned tree" ), & MdiTree::menuClicked );
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

	void PopupMenuViaButton( typename MdiTree< parent, MdiParent >::WidgetButtonPtr btn )
	{
		SmartWin::Rectangle btn_possize = btn->getBounds();

		itsPopmenu->trackPopupMenu( this, btn_possize.pos.x, btn_possize.pos.y + btn_possize.size.y, TPM_LEFTALIGN | TPM_TOPALIGN );
	}

	void addNode( typename MdiTree< parent, MdiParent >::WidgetButtonPtr btn )
	{
		SmartUtil::tstring item = itsTree->getSelectedItemText();

		TreeViewNode dummy, sel_node, new_node;

		if ( itsTree->getNode( dummy, TVGN_CARET, sel_node ) )
		{}
		else
		{}
		new_node = itsTree->insertNode( _T( "new" ), sel_node, 2 );
		itsTree->ensureVisible( new_node );
		itsTree->editLabel( new_node );
	}

	bool validLabel( typename MdiTree< parent, MdiParent >::WidgetTreeViewPtr tree, const SmartUtil::tstring & proposed_label )
	{
		return true;
	}

	void HtmlFilenameFromUser( SmartUtil::tstring & txtfilename )
	{
		typename MdiTree< parent, MdiParent >::WidgetLoadFile dlg = this->createLoadFile();
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

		if ( ! hg.get( itsTree ) )
		{
			this->createMessageBox().show( _T( "Tree parsing error." ), _T( "HTML parse error" ) );
		}
	}

	void FileSaveAs()
	{
		typename MdiTree< parent, MdiParent >::WidgetSaveFile dlg = this->createSaveFile();
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

		hp.put( itsTree );
	}

	void init_font()
	{
		sw::FontPtr font = createFont( _T( "Times New Roman" ), 20, 10, 1,
											 ANSI_CHARSET, false, false, false, 0, OUT_DEFAULT_PRECIS );

		itsTree->setFont( font );
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
			sw::FontPtr font = sw::createFont( lf.lfFaceName, lf.lfHeight, lf.lfWidth, lf.lfWeight,
												 lf.lfCharSet,
												 TRUE == lf.lfItalic, TRUE == lf.lfUnderline, TRUE == lf.lfStrikeOut );

			itsTree->setFont( font );
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

	void OnTextReady( const string & newtext )
	{}

	bool KeyPressed( typename MdiTree< parent, MdiParent >::WidgetTextBoxPtr textField, int key )
	{
		switch ( key )
		{
			case VK_RETURN :
				OnTextReady( itsInputBox->getText() );
				itsInputBox->setText( "" );
				return true;

			case VK_ESCAPE :
				//          itsAddBut->setText( "" );
				itsInputBox->setText( "" );
				return true;
			default: return false;
		}
		return true;
	}

	void treeNodeClicked( typename MdiTree< parent, MdiParent >::WidgetTreeViewPtr tree )
	{
		SmartUtil::tstring item = tree->getSelectedItemText();
		//    itsComboBox->setSelectedIndex( tree->getSelectedItemParam() );
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
		itsTree->setBounds( rc );

		itsMenuBut->setBounds( 0, rc.size.y, menu_size_x, menu_size_y );

		itsAddBut->setBounds( menu_size_x, rc.size.y, prompt_size_x, menu_size_y );

		itsInputBox->setBounds( menprom_size_x, rc.size.y, input_size_x, menu_size_y );
	}

public:
	MdiTree( parent * myparent, MdiParent * mdiParent,
			  void ( parent::* callbackfunc ) ( MdiTree * me ),
			  int base
			)
			: Widget( mdiParent ),
			itsParent( myparent ),
			m_close_callback_func( callbackfunc )
	{
		string trace;

		typename FactoryType::Seed seedMdiChild;

		Place myplace;
		myplace.setBoundsBorders( mdiParent->getSize(), 0, 0 );
		myplace.sizeOfCell( 1, 2, seedMdiChild.location.size );
		myplace.positionToRight( seedMdiChild.location );

		seedMdiChild.style = WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_OVERLAPPEDWINDOW;
		this->createMDIChild( seedMdiChild );

		onSized( & MdiTree< parent, MdiParent >::isResized );
		onClosing( & MdiTree< parent, MdiParent >::closeMdi );

		// Configure the prompt and input text box.
		itsMenuBut = this->createButton();
		itsMenuBut->setText( "menu" );
		itsMenuBut->onClicked( & MdiTree< parent, MdiParent >::PopupMenuViaButton );

		itsAddBut = this->createButton();
		itsAddBut->setText( _T( "Add" ) );
		itsAddBut->onClicked( & MdiTree< parent, MdiParent >::addNode );

		typename FactoryType::WidgetTextBox::Seed seedTextBox;
		seedTextBox.style = WS_VISIBLE | ES_WANTRETURN | ES_MULTILINE;
		itsInputBox = this->createTextBox( seedTextBox );
		itsInputBox->onKeyPressed( & MdiTree< parent, MdiParent >::KeyPressed );

		// Creating tree view
		itsTree = this->createTreeView();
		itsTree->setHasButtons( true );
		itsTree->setHasLines( true );
		itsTree->setLinesAtRoot( true );
		itsTree->setTrackSelect( true );
		itsTree->setEditLabels();
		itsTree->onClicked( & MdiTree< parent, MdiParent >::treeNodeClicked );
		itsTree->onValidateEditLabels( & MdiTree< parent, MdiParent >::validLabel );

		canned_tree();

		layout( seedTextBox.location.size ); // All Widgets must be created already.

		init_font();

		init_menu( base );
		onRightMouseDown( & MdiTree< parent, MdiParent >::PopupMenuViaRightClick );
	}

	void canned_tree()
	{
		TCHAR * cols[] =
		{ _T( "Alpha" ), _T( "Beta" ), _T( "Charlie" )
		};

		// Inserting items into tree view
		TreeViewNode root = itsTree->insertNode( _T( "Root" ), TreeViewNode(), - 1 );
		for ( int i = 0; i < 3; i++ ) itsTree->insertNode( cols[ i ], root, i );
	}

	~MdiTree()
	{}
};

#endif
