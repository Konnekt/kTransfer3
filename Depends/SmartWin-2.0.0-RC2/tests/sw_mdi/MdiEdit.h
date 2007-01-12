// $Revision: 1.8 $
/* Copyright (c) 2005 - Thomas Hansen
 * Part of the SmartWin++ library example code
 * Permission to use under the same terms as the actual library
 * The license to the library can be found at http://smartwinlib.org
 * License for library is Open Source and BSD
 */
#ifndef mdi_edit_38476547493_h
#define mdi_edit_38476547493_h

#include "SmartWin.h"
using namespace SmartWin;
#include "io/iolib.h"
#include <string>
using namespace std;
#include "io/InDialog.h"

/*
MdiEdit is a child of MdiFrame.

The MdiEdit class uses a WidgetTextBox as a general purpose editor.
The menu items allow normal editor operations.
*/

template< class parent, class MdiParent >
class MdiEdit
			: public SmartWin::WidgetFactory< SmartWin::WidgetMDIChild, MdiEdit< parent, MdiParent >, SmartWin::MessageMapPolicyMDIChildWidget >
{
public:

private:
	typedef SmartWin::WidgetFactory< SmartWin::WidgetMDIChild, MdiEdit< parent, MdiParent >, SmartWin::MessageMapPolicyMDIChildWidget > FactoryType;
	SmartUtil::tstring itsCurrentFilename;
	bool itsHexEditing;
	typename MdiEdit< parent, MdiParent >::WidgetTextBox * itsTextField;
	typename MdiEdit< parent, MdiParent >::WidgetButton * itsMenuBut;
	int itsBase;

	sw::FontPtr defaultfont();
	iolib itsIo;

	SmartUtil::tstring itsFtpServer, itsFtpFilename, itsFtpUsername;
	PasswordString itsFtpPassword;

	SmartUtil::tstring itsFindText;

	typename MdiEdit< parent, MdiParent >::WidgetMenuPtr itsMenu;
	typename MdiEdit< parent, MdiParent >::WidgetMenuPtr itsPopmenu;

	parent * itsParent;
	void ( parent::* m_close_callback_func ) ( MdiEdit * me );

	typedef enum MENU_ITEM
	{ OPEN = 1, SAVE, SAVE_AS, HEXEDIT,
							 PRINT, EMAIL, FTP_OPEN, FTP_SAVE, FONT,
							 CUT, COPY, PASTE, SEL_BY_FILE,
							 FIND_INPUT_TEXT, FIND_SEL_FORWARD, FIND_SEL_REVERSE
						   };

	void menuClicked( typename MdiEdit< parent, MdiParent >::WidgetMenuPtr menu, unsigned item )
	{
		switch ( item - itsBase )
		{
			case OPEN : FileOpen(); break;
			case SAVE : FileSaveCurrentFile(); break;
			case SAVE_AS : FileSaveAs(); break;
			case HEXEDIT : itsHexEditing = ! itsHexEditing;
				menu->checkItem( item, itsHexEditing );
				break;

			case EMAIL :
				{
					string emailtext = string( itsTextField->getText() );
					itsIo.FileEmailFile( emailtext );
				}
				break;

			case PRINT :
				{
					string printtext = string( itsTextField->getText() );
					itsIo.print( printtext );
				}
				break;

			case FTP_OPEN : FtpFileTransfer( true ); break;
			case FTP_SAVE : FtpFileTransfer( false ); break;

			case FONT : choose_font(); break;

			case CUT : cut(); break;
			case COPY : copy(); break;
			case PASTE : paste(); break;
			case SEL_BY_FILE : SelReplacedWithFile(); break;

			case FIND_INPUT_TEXT : find_input_text(); break;
			case FIND_SEL_FORWARD : find_selected_text(); break;
			case FIND_SEL_REVERSE : find_selected_text_rev(); break;
		}
	}

	void init_menu( int base )
	{
		itsBase = base;

		itsMenu = this->createMenu();
		itsPopmenu = itsMenu->appendPopup( _T( "&Menu" ) );

		typename MdiEdit< parent, MdiParent >::WidgetMenuPtr file = itsPopmenu->appendPopup( _T( "&File" ) );

		file->appendItem( OPEN + base, _T( "&Open" ), & MdiEdit< parent, MdiParent >::menuClicked );
		file->appendItem( SAVE + base, _T( "&Save" ), & MdiEdit< parent, MdiParent >::menuClicked );
		file->appendItem( SAVE_AS + base, _T( "Save &As" ), & MdiEdit< parent, MdiParent >::menuClicked );
		file->appendCheckedItem( HEXEDIT + base, _T( "&Hex editing" ), & MdiEdit< parent, MdiParent >::menuClicked );
		file->checkItem( HEXEDIT + base, false );
		file->appendSeparatorItem();
		file->appendItem( PRINT + base, _T( "&Print" ), & MdiEdit< parent, MdiParent >::menuClicked );
		file->appendItem( EMAIL + base, _T( "&Email" ), & MdiEdit< parent, MdiParent >::menuClicked );
		file->appendItem( FONT + base, _T( "&Font" ), & MdiEdit< parent, MdiParent >::menuClicked );

		typename MdiEdit< parent, MdiParent >::WidgetMenuPtr ftp = itsPopmenu->appendPopup( _T( "&Ftp" ) );
		ftp->appendItem( FTP_OPEN + base, _T( "&open" ), & MdiEdit< parent, MdiParent >::menuClicked );
		ftp->appendItem( FTP_SAVE + base, _T( "&save" ), & MdiEdit< parent, MdiParent >::menuClicked );

		typename MdiEdit< parent, MdiParent >::WidgetMenuPtr sel = itsPopmenu->appendPopup( _T( "&Selection" ) );
		sel->appendItem( CUT + base, _T( "Cu&t to clipboard" ), & MdiEdit< parent, MdiParent >::menuClicked );
		sel->appendItem( COPY + base, _T( "&Copy to clipboard" ), & MdiEdit< parent, MdiParent >::menuClicked );
		sel->appendItem( PASTE + base, _T( "&Paste from clipboard" ), & MdiEdit< parent, MdiParent >::menuClicked );
		sel->appendItem( SEL_BY_FILE + base, _T( "&Replace selection by file" ), & MdiEdit< parent, MdiParent >::menuClicked );
		sel->appendSeparatorItem();
		sel->appendItem( FIND_SEL_FORWARD + base, _T( "&Forward find" ), & MdiEdit< parent, MdiParent >::menuClicked );
		sel->appendItem( FIND_SEL_REVERSE + base, _T( "&Reverse find" ), & MdiEdit< parent, MdiParent >::menuClicked );
		sel->appendItem( FIND_INPUT_TEXT + base, _T( "&Specify find text" ), & MdiEdit< parent, MdiParent >::menuClicked );

		// itsMenu->attach();
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

	void PopupMenuViaButton( typename MdiEdit< parent, MdiParent >::WidgetButton * btn )
	{
		SmartWin::Rectangle btn_possize = btn->getBounds();

		itsPopmenu->trackPopupMenu( this, btn_possize.pos.x, btn_possize.pos.y + btn_possize.size.y, TPM_LEFTALIGN | TPM_TOPALIGN );
	}

	bool KeyPressed( typename MdiEdit< parent, MdiParent >::WidgetTextBox * textField, int key )
	{
		switch ( key )
		{
			case VK_F1 : find_selected_text(); return true;
			case VK_F2 : find_selected_text_rev(); return true;
			case VK_F3 : find_selected_text(); return true;
			default: return false;
		}
		return true;
	}

	void TxtFilenameFromUser( SmartUtil::tstring & txtfilename )
	{
		typename MdiEdit< parent, MdiParent >::WidgetLoadFile dlg = this->createLoadFile();
		dlg.addFilter( _T( "Text files (*.txt)" ), _T( "*.txt" ) );
		dlg.addFilter( _T( "All files (*.*)" ), _T( "*.*" ) );

		txtfilename = dlg.showDialog();
	}

	void FileOpen()
	{
		SmartUtil::tstring filePath;
		TxtFilenameFromUser( filePath );

		SmartUtil::tstring filedata;
		if ( itsIo.FileToString( filePath, filedata, itsHexEditing ) )
		{
			itsCurrentFilename = filePath;
			this->setText( itsCurrentFilename + _T( " - NotePad" ) );

			itsTextField->setText( filedata );
		}
	}

	void SelReplacedWithFile()
	{
		SmartUtil::tstring filePath;
		TxtFilenameFromUser( filePath );

		SmartUtil::tstring filedata;
		if ( itsIo.FileToString( filePath, filedata, itsHexEditing ) )
		{
			// Add file to the textedit Widget at the Caret position.
			long position = itsTextField->getCaretPos();
			itsTextField->replaceSelection( filedata );
		}
	}

	void FileSaveAs()
	{
		typename MdiEdit< parent, MdiParent >::WidgetSaveFile dlg = this->createSaveFile();
		dlg.addFilter( _T( "Text Files (*.txt)" ), _T( "*.txt" ) );
		dlg.addFilter( _T( "All Files (*.*)" ), _T( "*.*" ) );
		SmartUtil::tstring filePath = dlg.showDialog();
		itsCurrentFilename = filePath;
		this->setText( itsCurrentFilename + _T( " - NotePad" ) );

		FileSave( filePath );
	}

	void FileSaveCurrentFile()
	{
		if ( itsCurrentFilename.size() > 0 )
		{
			FileSave( itsCurrentFilename );
		}
	}

	void FileSave( const SmartUtil::tstring & filePath )
	{
		if ( itsIo.StringToFile( filePath, itsTextField->getText(), itsHexEditing ) )
		{
		}
	}

	void FtpFileTransfer( bool readfile )
	{
		SmartUtil::tstring title;
		if ( readfile )
		{
			title = "Read file using Ftp parameters of:";
		}
		else
		{
			title = "Write file using Ftp parameters of: ";
		}

		InDialog sd( this, title );
		sd.add( "Ftp server: ", & itsFtpServer )
		.add( "Filename: ", & itsFtpFilename )
		.add( "Username: ", & itsFtpUsername )
		.add( "Password: ", & itsFtpPassword );

		if ( IDOK == sd.createDialog() )
		{
			; // Blocks until the dialog is finished.
			if ( readfile )
			{
				// Read a FTP file and make it the current text.
				string ftptext;
				if ( itsIo.FtpReadFile( itsFtpServer, itsFtpFilename,
									   itsFtpUsername, itsFtpPassword.itsString,
									   ftptext ) )
				{
					itsTextField->setText( ftptext );
				}
				else
				{
					this->createMessageBox().show( "Ftp read failed", "FtpError" );
				}
			}
			else
			{
				// Write the current text to a FTP file.
				string ftptext = string( itsTextField->getText() );

				itsIo.FtpWriteFile( ftptext, itsFtpServer, itsFtpFilename,
								   itsFtpUsername, itsFtpPassword.itsString );
			}
		}
	}

	void cut()
	{
		SmartUtil::tstring selection = itsTextField->getSelection();
		SmartWin::ClipBoardString::instance().setClipBoardData( selection, this );
		itsTextField->replaceSelection( _T( "" ), true );
	}

	void copy()
	{
		SmartUtil::tstring selection = itsTextField->getSelection();
		SmartWin::ClipBoardString::instance().setClipBoardData( selection, this );
	}

	void paste()
	{
		if ( SmartWin::ClipBoardString::instance().isClipBoardFormatAvailable( SmartWin::ClipBoardBase::TEXT ) )
			itsTextField->replaceSelection( SmartWin::ClipBoardString::instance().getClipBoardData( this ) );
	}

	void find_input_text()
	{
		InDialog findDlg( this, "Find text" );
		findDlg.add( "Find what: ", & itsFindText );

		if ( IDOK == findDlg.createDialog() )
		{   // Blocks until the dialog is finished.
			findtext( itsFindText, 0 );
		}
	}

	void find_selected_text_rev()
	{
		SmartUtil::tstring selection = itsTextField->getSelection();
		long size = ( long ) selection.size();
		long position = itsTextField->getCaretPos() - ( size + 1 );
		if ( position < 0 ) return;

		const SmartUtil::tstring & txtOfBox = itsTextField->getText();
		position = ( long ) txtOfBox.rfind( selection, position );
		if ( position == - 1 )
			return;
		itsTextField->setSelection( position, position + size );
		itsTextField->setFocus();
		itsTextField->showCaret();
	}

	void find_selected_text()
	{
		SmartUtil::tstring selection = itsTextField->getSelection();
		find( selection );
	}

	void find( const SmartUtil::tstring & txt )
	{
		findtext( txt, itsTextField->getCaretPos() );
	}

	void findtext( const SmartUtil::tstring & txt, int start_pos )
	{
		long position = itsTextField->findText( txt, start_pos );
		if ( position == - 1 )
			return;
		itsTextField->setSelection( position, static_cast< long >( txt.size() ) + position );
		itsTextField->setFocus();
		itsTextField->showCaret();
	}

	void init_font()
	{
		sw::FontPtr font = createFont( _T( "Times New Roman" ), 20, 10, 1,
											 ANSI_CHARSET, false, false, false, 0, OUT_DEFAULT_PRECIS );

		//  FontPtr font = createFont( _T("Times New Roman"), 24, 0, 1,
		//              ANSI_CHARSET, false, false, false, 0, OUT_DEFAULT_PRECIS );

		itsTextField->setFont( font );
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

			// rgbPrev = SetTextColor(hdc, cf.rgbColors);

			itsTextField->setFont( font );
		}
	}

	//-------------------------------------------------------------------------
	/*
	IN:
	DOES:   Uses 
	OUT:    rc is set to client relative coordinates 0,0  sizex, sizey
	*/
	void layout( SmartWin::Point newsize )
	{
		const int but_height = 20;
		SmartWin::Rectangle rc_mdi_cli( newsize );

		SmartWin::Rectangle rc_edit = rc_mdi_cli; rc_edit.size.y -= but_height;
		itsTextField->setBounds( rc_edit );

		itsMenuBut->setBounds( 0, rc_edit.size.y, 60, but_height );
	}

	bool closeMdi()
	{
		// Invoke the callback function.
		// (m_callback_class.*m_close_callback_func)( *m_params );

		( itsParent->*m_close_callback_func ) ( this );
		return true;
	}

	void isResized( const SmartWin::WidgetSizedEventResult & sz )
	{
		layout( sz.newSize );
	}

public:
	MdiEdit( parent * myparent, MdiParent * mdiParent,
			  void ( parent::* callbackfunc ) ( MdiEdit * me ),
			  int base
			)
			: Widget( mdiParent ),
			itsParent( myparent ),
			m_close_callback_func( callbackfunc )
	{
		string trace;

		itsHexEditing = false;

		typename FactoryType::Seed seedMdiChild;

		Place myplace;
		myplace.setBoundsBorders( mdiParent->getSize(), 0, 0 );
		myplace.sizeOfCell( 1, 2, seedMdiChild.location.size );
		myplace.positionToRight( seedMdiChild.location );

		seedMdiChild.style = WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_OVERLAPPEDWINDOW;
		this->createMDIChild( seedMdiChild );

		//  setText( _T("Unsaved file - NotePad") );

		onSized( & MdiEdit< parent, MdiParent >::isResized );
		onClosing( & MdiEdit< parent, MdiParent >::closeMdi );

		itsMenuBut = this->createButton();
		itsMenuBut->setText( "menu" );
		itsMenuBut->onClicked( & MdiEdit< parent, MdiParent >::PopupMenuViaButton );

		// See documentation for Edit Control Styles from CreateWindow().
		typename FactoryType::WidgetTextBox::Seed seedTextBox;
		seedTextBox.style = WS_VISIBLE | ES_MULTILINE | ES_WANTRETURN | WS_VSCROLL; // | WS_HSCROLL
		seedTextBox.exStyle = 0;

		itsTextField = this->createTextBox( seedTextBox );
		itsTextField->onKeyPressed( & MdiEdit< parent, MdiParent >::KeyPressed );

		layout( this->getClientAreaSize() );

		onRightMouseDown( & MdiEdit< parent, MdiParent >::PopupMenuViaRightClick );

		init_font();
		init_menu( base );

		itsFtpServer = "upload.comcast.net";
		itsFtpFilename = "ftp.txt";
		itsFtpUsername = "andrew7webb";
	}

	~MdiEdit()
	{}

	void init()
	{}
};

#endif
