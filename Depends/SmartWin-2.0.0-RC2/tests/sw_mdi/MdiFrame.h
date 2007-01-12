// $Revision: 1.3 $
/* Copyright (c) 2005 - Thomas Hansen
 * Part of the SmartWin++ library example code
 * Permission to use under the same terms as the actual library
 * The license to the library can be found at http://smartwinlib.org
 * License for library is Open Source and BSD
 */
#ifndef mdi_frame_4757395743_h
#define mdi_frame_4757395743_h

#include "SmartWin.h"
using namespace SmartWin;
#include <string>
#include <vector>
using namespace std;
#include "MdiEdit.h"
#include "MdiGrid.h"
#include "MdiTree.h"
#include "MdiDraw.h"
#include "MdiWidg.h"
#include "wndextra.h"
// #include "place.h"

/*
	The MdiFrame class creates a window that manages the Mdichild windows via
	a main window menu that allow child window creation and manangement.

	It sizes them as large as possible in rows and columns.  It places them
	in order of creation.

	Each child must tell MdiFrame just before it closes.  MdiFrame keeps a
	list of all its children so it can present them correctly.
*/
template< class myclass >
class ChildrenList
{
public:
	std::vector < myclass * > itsMdiWindows;
	int itsTotalCreated;

	ChildrenList()
	{
		itsTotalCreated = 0;
	}

	void push_back( myclass * child )
	{
		itsTotalCreated++;
		itsMdiWindows.push_back( child );
	}

	int menu_base()
	{
		return ( 100 * itsTotalCreated );
	}

	void erase_if_find( myclass * child )
	{
		typename std::vector < myclass * >::iterator toBeRemoved =
			std::find( itsMdiWindows.begin(), itsMdiWindows.end(), child );

		itsMdiWindows.erase( toBeRemoved );
	}

	void RestoreAll()
	{
		for ( unsigned int i = 0; i < itsMdiWindows.size(); i++ )
		{
			itsMdiWindows[ i ] ->restore();
		}
	}

	void MinAll()
	{
		for ( unsigned int i = 0; i < itsMdiWindows.size(); i++ )
		{
			itsMdiWindows[ i ] ->minimize();
		}
	}
};

class MdiFrame
			: public SmartWin::WidgetFactory< SmartWin::WidgetWindow, MdiFrame >
{
	enum layout
	{
		ONE_COL = 1, DUAL_COLS, ONE_ROW, DUAL_ROWS, RIGHTAFTER, NONE
	};

public:

private:
	enum layout itsLayout;

	WidgetMenuPtr itsMenu;
	WidgetMDIParentPtr itsMdiParent;

	ChildrenList< MdiGrid< MdiFrame, WidgetMDIParent > > itsGrids;
	ChildrenList< MdiTree< MdiFrame, WidgetMDIParent > > itsTrees;
	ChildrenList< MdiEdit< MdiFrame, WidgetMDIParent > > itsEdits;
	ChildrenList< MdiDraw< MdiFrame, WidgetMDIParent > > itsDraws;
	ChildrenList< MdiWidg< MdiFrame, WidgetMDIParent > > itsWidgs;

	std::vector< HWND > itsHands;

	void init_menu()
	{
		itsMenu = createMenu();

		int id = 50; // High enough to prevent interference from predefined menu items?

		WidgetMenuPtr create = itsMenu->appendPopup( _T( "&Creation" ) );
		create->appendItem( id++, _T( "&edit" ), & MdiFrame::create_edit );
		create->appendItem( id++, _T( "&grid" ), & MdiFrame::create_grid );
		create->appendItem( id++, _T( "&tree" ), & MdiFrame::create_tree );
		create->appendItem( id++, _T( "&draw" ), & MdiFrame::create_draw );
		create->appendItem( id++, _T( "&widgets" ), & MdiFrame::create_widg );

		WidgetMenuPtr wins = itsMenu->appendPopup( _T( "&Windows" ) );
		WidgetMenuPtr layout = wins->appendPopup( _T( "&Fit for" ) );
		layout->appendItem( id++, _T( "Dual rows" ), & MdiFrame::menuDualRows );
		layout->appendItem( id++, _T( "Dual columns" ), & MdiFrame::menuDualCols );
		layout->appendItem( id++, _T( "One column" ), & MdiFrame::menuOneCol );
		layout->appendItem( id++, _T( "One row" ), & MdiFrame::menuOneRow );
		layout->appendItem( id++, _T( "User set sizes" ), & MdiFrame::menuPreserveSize );

		wins->appendItem( id++, _T( "&Restore all" ), & MdiFrame::menuRestoreAll );
		wins->appendItem( id++, _T( "&Minimize all" ), & MdiFrame::menuMinAll );
		wins->appendSeparatorItem();
		wins->appendItem( id++, _T( "&No Layout" ), & MdiFrame::menuNoLayout );

		itsMenu->attach( this );
	}

	void menuDualRows( WidgetMenuPtr menu, unsigned item )
	{
		itsLayout = DUAL_ROWS;
		wantResized();
	}

	void menuOneRow( WidgetMenuPtr menu, unsigned item )
	{
		itsLayout = ONE_ROW;
		wantResized();
	}

	void menuOneCol( WidgetMenuPtr menu, unsigned item )
	{
		itsLayout = ONE_COL;
		wantResized();
	}

	void menuDualCols( WidgetMenuPtr menu, unsigned item )
	{
		itsLayout = DUAL_COLS;
		wantResized();
	}

	void menuNoLayout( WidgetMenuPtr menu, unsigned item )
	{
		itsLayout = NONE;
		wantResized();
	}

	void menuPreserveSize( WidgetMenuPtr menu, unsigned item )
	{
		itsLayout = RIGHTAFTER;
		wantResized();
	}

	void wantResized()
	{
		SmartWin::WidgetSizedEventResult sz;
		sz.isMaximized = false;
		sz.isMinimized = false;
		sz.isRestored = false;

		SmartWin::Rectangle rc( itsMdiParent->getSize() ); // Trigger a resize event

		sz.newSize = rc.size;
		isResized( sz );

		// setBounds( rc );                     // so we can adjust for the change.
	}

	void isResized( const SmartWin::WidgetSizedEventResult & sz )
	{
		itsMdiParent->setBounds( 0, 0, sz.newSize.x, sz.newSize.y );

		if ( NONE == itsLayout ) return; // Nothing more to do

		int numwins = ( int ) itsHands.size();
		if ( 0 == numwins ) return;

		// Now place the various MDI client windows
		Place p;
		// int xborder= 4, yborder= 8;
		int xborder = 0, yborder = 0;
		p.setBoundsBorders( itsMdiParent->getClientAreaSize(), xborder, yborder );

		sw::Rectangle pos_size;

		if ( RIGHTAFTER == itsLayout )
		{
			// Place the windows from left to right, preserving their size.
			for ( int i = 0; i < numwins; i++ )
			{
				wndextra win( itsHands[ i ] );
				pos_size = win.getSize();
				p.positionToRight( pos_size );
				win.setBounds( pos_size );
			}
			return;
		}

		int rows, cols;
		switch ( itsLayout )
		{
			case DUAL_COLS : rows = ( numwins + 1 ) / 2; cols = 2; break;
			case DUAL_ROWS : rows = 2; cols = ( numwins + 1 ) / 2; break;
			case ONE_COL : rows = numwins; cols = 1; break;
			case ONE_ROW : rows = 1; cols = numwins; break;
		}

		// Divide the mdi parent into rows*cols windows
		p.sizeOfCell( rows, cols, pos_size.size );

		// Use the same size for each window, as calculated above.
		int i;
		for ( i = 0; i < numwins - 1; i++ )
		{
			p.positionToRight( pos_size );
			wndextra win( itsHands[ i ] );
			win.setBounds( pos_size );
		}

		// Make the last window take all the rest of the space.
		p.sizeForRemainingSpace( pos_size );
		wndextra win( itsHands[ i ] );
		win.setBounds( pos_size );
	}

	void menuMinAll( WidgetMenuPtr menu, unsigned item )
	{
		itsGrids.MinAll();
		itsEdits.MinAll();
		itsTrees.MinAll();
		itsDraws.MinAll();
		itsWidgs.MinAll();
	}

	void menuRestoreAll( WidgetMenuPtr menu, unsigned item )
	{
		itsGrids.RestoreAll();
		itsEdits.RestoreAll();
		itsTrees.RestoreAll();
		itsDraws.RestoreAll();
		itsWidgs.RestoreAll();

		wantResized();
	}

	void RemoveHWND( HWND w )
	{
		std::vector< HWND >::iterator toBeRemoved =
			std::find( itsHands.begin(), itsHands.end(), w );

		itsHands.erase( toBeRemoved );
	}

	//-------------------------------------------------------------------------
	// todo: figure out how to consolidate these.
	void gridClosed( MdiGrid< MdiFrame, WidgetMDIParent > * me )
	{
		itsGrids.erase_if_find( me );
		RemoveHWND( me->handle() );
		wantResized();
	}

	void editClosed( MdiEdit< MdiFrame, WidgetMDIParent > * me )
	{
		itsEdits.erase_if_find( me );
		RemoveHWND( me->handle() );
		wantResized();
	}

	void treeClosed( MdiTree< MdiFrame, WidgetMDIParent > * me )
	{
		itsTrees.erase_if_find( me );
		RemoveHWND( me->handle() );
		wantResized();
	}

	void drawClosed( MdiDraw< MdiFrame, WidgetMDIParent > * me )
	{
		itsDraws.erase_if_find( me );
		RemoveHWND( me->handle() );
		wantResized();
	}

	void widgClosed( MdiWidg< MdiFrame, WidgetMDIParent > * me )
	{
		itsWidgs.erase_if_find( me );
		RemoveHWND( me->handle() );
		wantResized();
	}

	int uniquebase()
	{
		return ( ( int ) ( 100 * ( 1 + itsHands.size() ) ) );
	}

	void create_edit( WidgetMenuPtr menu, unsigned item )
	{
		MdiEdit< MdiFrame, WidgetMDIParent > * me;

		me = new MdiEdit< MdiFrame, WidgetMDIParent >
			 ( this, itsMdiParent, & MdiFrame::editClosed, uniquebase() );
		itsEdits.push_back( me );
		itsHands.push_back( me->handle() );
		me->setText( "Edit Widget" );

		wantResized();
	}

	void create_tree( WidgetMenuPtr menu, unsigned item )
	{
		MdiTree< MdiFrame, WidgetMDIParent > * me;

		me = new MdiTree< MdiFrame, WidgetMDIParent >
			 ( this, itsMdiParent, & MdiFrame::treeClosed, uniquebase() );
		itsTrees.push_back( me );
		itsHands.push_back( me->handle() );
		me->setText( "Tree Widget" );

		wantResized();
	}

	void create_grid( WidgetMenuPtr menu, unsigned item )
	{
		MdiGrid< MdiFrame, WidgetMDIParent > * me;

		me = new MdiGrid< MdiFrame, WidgetMDIParent >
			 ( this, itsMdiParent, & MdiFrame::gridClosed, uniquebase() );

		me->setText( "Grid Widget" );
		itsGrids.push_back( me );
		itsHands.push_back( me->handle() );
		wantResized();
	}

	void create_draw( WidgetMenuPtr menu, unsigned item )
	{
		MdiDraw< MdiFrame, WidgetMDIParent > * me;

		me = new MdiDraw< MdiFrame, WidgetMDIParent >
			 ( this, itsMdiParent, & MdiFrame::drawClosed, uniquebase() );

		me->setText( "Draw Widget" );
		itsDraws.push_back( me );
		itsHands.push_back( me->handle() );
		wantResized();
	}

	void create_widg( WidgetMenuPtr menu, unsigned item )
	{
		MdiWidg< MdiFrame, WidgetMDIParent > * me;

		me = new MdiWidg< MdiFrame, WidgetMDIParent >
			 ( this, itsMdiParent, & MdiFrame::widgClosed, uniquebase() );

		me->setText( "Widget" );
		itsWidgs.push_back( me );
		itsHands.push_back( me->handle() );
		wantResized();
	}

public:
	MdiFrame()
	{
		itsLayout = DUAL_ROWS;
	}

	~MdiFrame()
	{}

	void init()
	{
		createWindow();

		setText( _T( "MDI Frame" ) );

		onSized( & MdiFrame::isResized );

		WidgetMDIParent::Seed cs;
		cs.exStyle = WS_EX_CLIENTEDGE;
		itsMdiParent = createMDIParent( cs );
		// setBounds( 100, 100, 600, 600 );

		init_menu();
	}
};

#endif
