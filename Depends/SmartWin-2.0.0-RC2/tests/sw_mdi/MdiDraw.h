// $Revision: 1.6 $
/* Copyright (c) 2005 - Thomas Hansen
 * Part of the SmartWin++ library example code
 * Permission to use under the same terms as the actual library
 * The license to the library can be found at http://smartwinlib.org
 * License for library is Open Source and BSD
 */
#ifndef mdi_draw_88342_h
#define mdi_draw_88342_h

#include "SmartWin.h"
using namespace SmartWin;
#include <string>
using namespace std;

/*
MdiDraw is a child of MdiFrame.

The MdiDraw class uses the Win32 GDI to draw a few shapes.  The Win32
"enhanced meta file" holds the GDI commands for playback as needed in the 
painting function.  The drawing can can be stored as a .emf file, or a new drawing may be loaded from any .emf.
*/

template< class parent, class MdiParent >
class MdiDraw
			: public SmartWin::WidgetFactory< SmartWin::WidgetMDIChild, MdiDraw< parent, MdiParent >, SmartWin::MessageMapPolicyMDIChildWidget >
{
public:

private:
	typedef SmartWin::WidgetFactory< SmartWin::WidgetMDIChild, MdiDraw< parent, MdiParent >, SmartWin::MessageMapPolicyMDIChildWidget > FactoryType;

	HENHMETAFILE itsHEmf;

	typename MdiDraw< parent, MdiParent >::WidgetMenuPtr itsMenu, itsPopmenu;

	int itsBase;

	parent * itsParent;
	void ( parent::* m_close_callback_func ) ( MdiDraw * me );

	typedef enum MENU_ITEM
	{
		OPEN = 1, SAVE, SAVE_AS, PRINT
	};

	void menuClicked( typename MdiDraw< parent, MdiParent >::WidgetMenuPtr menu, unsigned item )
	{
		switch ( item - itsBase )
		{
			case OPEN : FileOpen(); break;
			case SAVE :              // FileSaveCurrentFile();   break;
			case SAVE_AS : FileSaveAs(); break;
		}
	}

	void init_menu( int base )
	{
		itsBase = base;

		itsMenu = this->createMenu();
		itsPopmenu = itsMenu->appendPopup( _T( "&Menu" ) );

		typename MdiDraw< parent, MdiParent >::WidgetMenuPtr file = itsPopmenu->appendPopup( _T( "&Enhanced Meta File" ) );

		file->appendItem( OPEN + base, _T( "&Open" ), & MdiDraw::menuClicked );
		file->appendItem( SAVE + base, _T( "&Save" ), & MdiDraw::menuClicked );
		file->appendItem( SAVE_AS + base, _T( "Save &As" ), & MdiDraw::menuClicked );
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

	void painting( SmartWin::Canvas & canvas )
	{
		// Convert from rc.pos= upper left, rc.size= lower right.
		//         into rc.pos= 0,0       , rc.size= width, height
		SmartWin::Rectangle rc( this->getClientAreaSize() );

		rc.pos.x = rc.pos.y = 0;

		RECT rect;
		rect.left = 0; rect.top = 0;
		rect.right = rc.size.x; rect.bottom = rc.size.y;
		if ( PlayEnhMetaFile( canvas.getDc(), itsHEmf, & rect ) )
		{}
	}

	void paint( HDC hdc, int xlim, int ylim )
	{
		int x2 = xlim / 2, x4 = x2 / 2, y2 = ylim / 2, y4 = y2 / 2;

		COLORREF pencolor = RGB( rand() % 255, rand() % 255, rand() % 255 ),
							brushcolor = RGB( rand() % 255, rand() % 255, rand() % 255 );
		{   // With one pen color.
			// SmartWin::Pen  pen( hdc, pencolor );
			::SelectObject( hdc, CreatePen( PS_SOLID, 3, pencolor ) );

			::MoveToEx( hdc, 0, 0, 0 );
			::LineTo( hdc, x2, y2 ); ::LineTo( hdc, 0, y2 );
			::LineTo( hdc, x4, y4 ); ::LineTo( hdc, x4, y2 );
		}

		// With the Brush and pen color the same
		// SmartWin::Pen  pen( hdc, brushcolor );
		::SelectObject( hdc, CreatePen( PS_SOLID, 1, brushcolor ) );
		::SelectObject( hdc, CreateSolidBrush( brushcolor ) );
		::Ellipse( hdc, 0, y2, xlim, ylim );

		::Rectangle( hdc, x2, y4, xlim, y2 );

		::SetPixel( hdc, 10, 10, pencolor );
		::ExtFloodFill( hdc, 11, 11, GetPixel( hdc, 11, 11 ), FLOODFILLSURFACE );
	}

	void EmfFilenameFromUser( SmartUtil::tstring & txtfilename )
	{
		typename MdiDraw< parent, MdiParent >::WidgetLoadFile dlg = this->createLoadFile();
		dlg.addFilter( _T( "Enhanced meta files (*.emf)" ), _T( "*.emf" ) );
		dlg.addFilter( _T( "All files (*.*)" ), _T( "*.*" ) );

		txtfilename = dlg.showDialog();
	}

	void FileOpen()
	{
		SmartUtil::tstring filePath;
		EmfFilenameFromUser( filePath );

		itsHEmf = GetEnhMetaFile( filePath.c_str() );
		this->updateWidget();
	}

	void FileSaveAs()
	{
		typename MdiDraw< parent, MdiParent >::WidgetSaveFile dlg = this->createSaveFile();
		dlg.addFilter( _T( "Enhanced meta files (*.emf)" ), _T( "*.emf" ) );
		dlg.addFilter( _T( "All files (*.*)" ), _T( "*.*" ) );
		SmartUtil::tstring filePath = dlg.showDialog();
		//  setText( current_filename + _T(" - Draw") );

		CreateCannedEmf( filePath.c_str() );
	}

	void CreateCannedEmf( const char * filename )
	{
		SmartWin::Rectangle rc = sw::Rectangle( this->getClientAreaSize() );
		RECT r;
		r.left = 0; r.top = 0;
		int xlim = r.right = rc.size.x;
		int ylim = r.bottom = rc.size.y;

		UpdateCanvas uc( this );
		map10um( uc.getDc(), r );

		HDC enhhdc = CreateEnhMetaFile( uc.getDc(), filename, & r, "description" );
		paint( enhhdc, xlim, ylim );
		itsHEmf = CloseEnhMetaFile( enhhdc );
	}

	void to_10um( HDC hDC, long & coord, int size, int res )
	{
		int dimmm = GetDeviceCaps( hDC, size );
		int pixdim = GetDeviceCaps( hDC, res );

		coord = ( coord * dimmm * 100 + pixdim / 2 ) / pixdim;
	}

	void map10um( HDC hDC, RECT & rect )
	{
		LPtoDP( hDC, ( POINT * ) & rect, 2 );

		rect.left = 0; // to_10um( hDC, rect.left, HORZSIZE, HORZRES );
		rect.top = 0; // to_10um( hDC, rect.top , VERTSIZE, VERTRES );
		to_10um( hDC, rect.right, HORZSIZE, HORZRES );
		to_10um( hDC, rect.bottom, VERTSIZE, VERTRES );
	}

	bool closeMdi()
	{
		// Invoke the callback function.

		( itsParent->*m_close_callback_func ) ( this );
		return true;
	}

	void isResized( const SmartWin::WidgetSizedEventResult & sz )
	{
		this->updateWidget();
	}

	void KeyPressed( typename MdiDraw< parent, MdiParent >::WidgetTextBoxPtr textField, int key )
	{
		switch ( key )
		{
			case VK_RETURN :
				break;

			case VK_ESCAPE :
				break;
		}
	}

public:
	MdiDraw( parent * myparent, MdiParent * mdiParent,
			  void ( parent::* callbackfunc ) ( MdiDraw * me ),
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

		CreateCannedEmf( 0 );

		onSized( & MdiDraw< parent, MdiParent >::isResized );
		onPainting( & MdiDraw< parent, MdiParent >::painting );
		onClosing( & MdiDraw< parent, MdiParent >::closeMdi );

		init_menu( base );
		onRightMouseDown( & MdiDraw< parent, MdiParent >::PopupMenuViaRightClick );
	}

	~MdiDraw()
	{
		DeleteEnhMetaFile( itsHEmf );
	}
};

#endif
