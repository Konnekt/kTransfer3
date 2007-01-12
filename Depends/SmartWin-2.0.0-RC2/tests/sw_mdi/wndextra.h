// $Revision: 1.4 $
/* Copyright (c) 2005 - Thomas Hansen
 * Part of the SmartWin++ library example code
 * Permission to use under the same terms as the actual library
 * The license to the library can be found at http://smartwinlib.org
 * License for library is Open Source and BSD
 */
#ifndef wndextra_854763_h
#define wndextra_854763_h

#include "SmartWin.h"

/*
Some window helping functions given a HWND
-- With time and rethinking, could probably use SmartWin functions also.
*/
class wndextra
{
public:
	HWND itsHand;

	wndextra( HWND hand )
	{
		itsHand = hand;
	}

	//---------------------------------------------------------

	SmartWin::Rectangle getSize()
	{
		WINDOWINFO wi;
		wi.cbSize = sizeof( WINDOWINFO );
		if ( ! GetWindowInfo( itsHand, & wi ) )
		{
			return ( SmartWin::Rectangle( 0, 0, 0, 0 ) );
		}

		return ( SmartWin::Rectangle( wi.rcWindow.left, wi.rcWindow.top,
									  wi.rcWindow.right - wi.rcWindow.left,
									  wi.rcWindow.bottom - wi.rcWindow.top ) );
	}

	//---------------------------------------------------------

	void setBounds( SmartWin::Rectangle & rect )
	{
		::MoveWindow( itsHand, rect.pos.x, rect.pos.y,
					  rect.size.x, rect.size.y, TRUE );
	}

	//---------------------------------------------------------

	bool WinIs( unsigned int state )
	{
		WINDOWPLACEMENT wndpl;
		wndpl.length = sizeof( WINDOWPLACEMENT );
		if ( ! GetWindowPlacement( itsHand, & wndpl ) ) return ( false );

		switch ( wndpl.showCmd )
		{
			case SW_NORMAL : break; // 1
			case SW_SHOWMINIMIZED :  // 2
			case SW_MAXIMIZE :       // 3
			case SW_MINIMIZE :       // 6
			case SW_FORCEMINIMIZE :  // 6
				int n = n;
		};

		/*
		#define SW_SHOWNOACTIVATE   4
		#define SW_SHOW             5
		#define SW_MINIMIZE         6
		#define SW_SHOWMINNOACTIVE  7
		#define SW_SHOWNA           8
		#define SW_RESTORE          9
		#define SW_SHOWDEFAULT      10
		#define SW_FORCEMINIMIZE    11
		#define SW_MAX              11
		*/

		return ( wndpl.showCmd == state );
	}

	//-------------------------------------------------------------------------
	//  int numwins = CountNonMinimized( ); // (int) edits.mdiWindows.size();
	//      if( ! WinIs( edits.mdiWindows[i]->handle(), SW_SHOWMINIMIZED ) ) {
	int CountNonMinimized()
	{
		int n = 0;
		/*  for ( unsigned int i=0; i< grids.mdiWindows.size(); i++ ) {
				if( ! WinIs( grids.mdiWindows[i]->handle(), SW_SHOWMINIMIZED ) ) {
					n++;
				}
			}
		*/ 
		return ( n );
	}

	//---------------------------------------------------------
};

#endif
