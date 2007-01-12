// $Revision: 1.7 $
/* Copyright (c) 2005 - Thomas Hansen
 * Part of the SmartWin++ library example code
 * Permission to use under the same terms as the actual library
 * The license to the library can be found at http://smartwinlib.org
 * License for library is Open Source and BSD
 */
#ifndef mdi_widg_0587684_h
#define mdi_widg_0587684_h

#include "SmartWin.h"
using namespace SmartWin;
#include <string>
using namespace std;

/*
MdiWidg is a itsChildWin of MdiFrame.
The MdiWidg class demonstrates the use of the various Widgets.
*/
template< class parent, class MdiParent >
class MdiWidg
			: public SmartWin::WidgetFactory< SmartWin::WidgetMDIChild, MdiWidg< parent, MdiParent >, SmartWin::MessageMapPolicyMDIChildWidget >
{
public:
	typename MdiWidg< parent, MdiParent >::WidgetTabSheet * itsTabSheet;

private:
	typedef SmartWin::WidgetFactory< SmartWin::WidgetMDIChild, MdiWidg< parent, MdiParent >, SmartWin::MessageMapPolicyMDIChildWidget > FactoryType;
	typename MdiWidg< parent, MdiParent >::WidgetDataGridPtr itsGrid;
	typename MdiWidg< parent, MdiParent >::WidgetButtonPtr itsMenuBut;
	typename MdiWidg< parent, MdiParent >::WidgetStaticPtr itsPrompt;
	typename MdiWidg< parent, MdiParent >::WidgetTextBoxPtr itsInputBox;
	typename MdiWidg< parent, MdiParent >::WidgetComboBoxPtr itsComboBox;
	typename MdiWidg< parent, MdiParent >::WidgetChildWindowPtr itsButtonPage, itsComboPage, itsTextBoxPage;
	int itsBase;

	sw::FontPtr defaultfont();

	typename MdiWidg< parent, MdiParent >::WidgetMenuPtr itsMenu;
	typename MdiWidg< parent, MdiParent >::WidgetMenuPtr itsPopmenu;

	parent * itsParent;
	void ( parent::* m_close_callback_func ) ( MdiWidg * me );

	typedef enum MENU_ITEM
	{
		OPEN = 1, SAVE, SAVE_AS, PRINT, EMAIL, FONT
	};

	void menuClicked( typename MdiWidg< parent, MdiParent >::WidgetMenuPtr menu, unsigned item )
	{
		switch ( item - itsBase )
		{
			case OPEN :              //FileOpen();               break;
			case SAVE :              //FileSaveCurrentFile();    break;
			case SAVE_AS :           //FileSaveAs();         break;

			case EMAIL :
				{
					// string emailtext = string( textField->getText() );
					// myio.FileEmailFile( emailtext );
				}
				break;

			case PRINT :
				{
					// string printtext = string( textField->getText() );
					// myio.print( printtext );
				}
				break;

			case FONT : choose_font(); break;
		}
	}

	void init_menu( int base )
	{
		itsBase = base;

		itsMenu = this->createMenu();
		itsPopmenu = itsMenu->appendPopup( _T( "&Menu" ) );

		typename MdiWidg< parent, MdiParent >::WidgetMenuPtr file = itsPopmenu->appendPopup( _T( "&File" ) );

		file->appendItem( OPEN + base, _T( "&Open" ), & MdiWidg::menuClicked );
		file->appendItem( SAVE + base, _T( "&Save" ), & MdiWidg::menuClicked );
		file->appendItem( SAVE_AS + base, _T( "Save &As" ), & MdiWidg::menuClicked );
		file->appendSeparatorItem();
		file->appendItem( PRINT + base, _T( "&Print" ), & MdiWidg::menuClicked );
		file->appendItem( EMAIL + base, _T( "&Email" ), & MdiWidg::menuClicked );
		file->appendItem( FONT + base, _T( "&Font" ), & MdiWidg::menuClicked );
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

	void PopupMenuViaButton( typename MdiWidg< parent, MdiParent >::WidgetButtonPtr btn )
	{
		SmartWin::Rectangle btn_possize( btn->getSize() );

		::TrackPopupMenu( reinterpret_cast< HMENU >( itsPopmenu->handle() ),
						  TPM_LEFTALIGN | TPM_TOPALIGN,
						  btn_possize.pos.x, btn_possize.pos.y + btn_possize.size.y,
						  0, this->handle(), 0 );
	}

	void new_tab( typename MdiWidg< parent, MdiParent >::WidgetTabSheetPtr sheet )
	{
		int newdex = sheet->getSelectedIndex();

		itsButtonPage->setVisible( newdex == 0 );
		itsComboPage->setVisible( newdex == 1 );
		itsTextBoxPage->setVisible( newdex == 2 );
	}

	void init_font()
	{
		sw::FontPtr font = createFont( _T( "Times New Roman" ), 20, 10, 1,
											 ANSI_CHARSET, false, false, false, 0, OUT_DEFAULT_PRECIS );

		itsTabSheet->setFont( font );
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
		layout();
	}

	bool KeyPressed( typename MdiWidg< parent, MdiParent >::WidgetTextBoxPtr textField, int key )
	{
		switch ( key )
		{
			case VK_RETURN :
				//          OnTextReady( InputBox->getText() );
				itsInputBox->setText( "" );
				return true;

			case VK_ESCAPE :
				itsPrompt->setText( "" );
				itsInputBox->setText( "" );
				return true;
			default: return false;
		}
		return true;
	}

	//-------------------------------------------------------------------------
	/*
	IN:
	DOES:   Uses 
	OUT:    rc is set to client relative coordinates 0,0  sizex, sizey
	*/
	void layout()
	{
		SmartWin::Rectangle rc( this->getClientAreaSize() );
		itsTabSheet->setBounds( rc );
	}

public:
	MdiWidg( parent * myparent, MdiParent * mdiParent,
			  void ( parent::* callbackfunc ) ( MdiWidg * me ),
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

		itsTabSheet = this->createTabSheet();
		itsTabSheet->addPage( "buttons", 0 );
		itsButtonPage = this->createWidgetChildWindow();
		itsTabSheet->addPage( "combolists", 1 );
		itsComboPage = this->createWidgetChildWindow();
		itsTabSheet->addPage( "textboxes", 2 );
		itsTextBoxPage = this->createWidgetChildWindow();
		itsTabSheet->onSelectionChanged( & MdiWidg< parent, MdiParent >::new_tab );
		itsTabSheet->setHotTrack();
		//  itsTabSheet->setTabsAtBottom();

		// error C2440: '=' : cannot convert
		// from 'SmartWin::WidgetFactory<Base,Parent,WidgetMessageMapType>::WidgetComboBoxPtr'
		//   to 'SmartWin::WidgetFactory<Base,Parent,WidgetMessageMapType>::WidgetComboBoxPtr'
		//         with
		//        [
		//            Base=SmartWin::WidgetWindow,
		//            Parent=MdiWidg<MdiFrame,SmartWin::WidgetFactory<SmartWin::WidgetWindow,MdiFrame>::WidgetMDIParent>,
		//            WidgetMessageMapType=SmartWin::NormalWidget
		//        ]
		//        and
		//        [
		//            Base=SmartWin::WidgetMDIChild,
		//            Parent=MdiWidg<MdiFrame,SmartWin::WidgetFactory<SmartWin::WidgetWindow,MdiFrame>::WidgetMDIParent>,
		//            WidgetMessageMapType=SmartWin::MessageMapPolicyMDIChildWidget
		//        ]
		//        Types pointed to are unrelated; conversion requires reinterpret_cast, C-style cast or function-style cast
		//
#ifdef false
		itsComboBox = itsComboPage->createComboBox(); // gives error above
#endif

		// Configure the itsPrompt and input text box.
		itsMenuBut = this->createButton();
		itsMenuBut->setText( "menu" );
		itsMenuBut->onClicked( & MdiWidg< parent, MdiParent >::PopupMenuViaButton );

		typename FactoryType::WidgetStatic::Seed seedStatic;
		seedStatic.style = ES_RIGHT;
		itsPrompt = this->createStatic( seedStatic );

		itsInputBox = this->createTextBox();
		itsInputBox->onKeyPressed( & MdiWidg< parent, MdiParent >::KeyPressed );

		typename FactoryType::WidgetDataGrid::Seed seedGrid;
		seedGrid.style = WS_HSCROLL | WS_VSCROLL;
		seedGrid.exStyle = 0;
		itsGrid = this->createDataGrid( seedGrid );

		//  itsGrid->createColumns( colnames );
		itsGrid->setGridLines();
		itsGrid->setHeaderDragDrop();
		itsGrid->setFullRowSelect();

		init_font();
		init_menu( base );

		layout();
		this->onSized( & MdiWidg< parent, MdiParent >::isResized );
		this->onClosing( & MdiWidg< parent, MdiParent >::closeMdi );
		this->onRightMouseDown( & MdiWidg< parent, MdiParent >::PopupMenuViaRightClick );
	}

	~MdiWidg()
	{}
};

#endif
