// $Revision: 1.7 $
/* Copyright (c) 2005 - Thomas Hansen
 * Part of the SmartWin++ library example code
 * Permission to use under the same terms as the actual library
 * The license to the library can be found at http://smartwinlib.org
 * License for library is Open Source and BSD
 */ 
/*  A little game just for fun.  (ok, its also a SmartWin demo )
 */
#include "SmartWin.h"
using namespace SmartWin;
#include <queue>
#include <vector>

class Encircle
			: public WidgetFactory< WidgetWindow, Encircle >
{
	enum
	{
		heartbeat = 100
	};

public:
	void init()
	{
		createWindow();
		setText( _T( "Encircle" ) );

		itsColor = RandColor();

		UpdateCanvas uc( this );
		itsBackgroundColor = uc.getPixel( 0, 0 );
		itsDrawWidth = 25;
		itsEraseWidth = 25;
		itsHist_max = 200;

		onSized( & Encircle::isResized );
		onPainting( & Encircle::painting );
		onMouseMove( & Encircle::MouseMoveResponse );
		onKeyPressed( & Encircle::KeyPressed );

		Command cmd( _T( "Testing" ) ); // Must do it like this since due to private Constructor gcc complains if created within actual function call
		createTimer( & Encircle::heartbeatResponse, heartbeat, cmd );
	}

private:
	unsigned itsHist_max;
	std::queue< Point > itsHist;
	COLORREF itsColor, itsBackgroundColor;
	int itsDrawWidth, itsEraseWidth;

	COLORREF RandColor()
	{
		return ( RGB( rand() % 255, rand() % 255, rand() % 255 ) );
	}

	//  Called when the window needs to be redrawn
	//
	void painting( SmartWin::Canvas & c )
	{
		/*      size_t histsize= itsHist.size();

				if ( histsize < 2 ) return;

				c.moveTo( itsHist.front() );
				itsHist.push( itsHist.pop() );

				size_t histsize2= itsHist.size();

				for( size_t i= 0; i < itsHist.size() - 1; i++ ) {
					c.lineTo( itsHist.front() );
					itsHist.push( itsHist.pop() );
				}
		*/
	}

	// There are new dimensions to the window because the user resized the window.
	//
	void isResized( const SmartWin::WidgetSizedEventResult & sz )
	{
		updateWidget();
	}

	void MouseMoveResponse( const SmartWin::MouseEventResult & mouse )
	{
		if ( mouse.ButtonPressed == SmartWin::MouseEventResult::LEFT )
		{}

		if ( mouse.ButtonPressed == SmartWin::MouseEventResult::RIGHT )
		{
			itsColor = RandColor();
		}

		if ( TouchingRope( mouse.pos ) )
		{
			itsColor = RandColor();
		}

		AdjustRope( mouse.pos );
	}

	bool TouchingRope( Point mousepos )
	{
		UpdateCanvas uc( this );

		COLORREF c = uc.getPixel( mousepos );
		if ( c == itsBackgroundColor )
		{
			return ( false );
		}

		return ( false );
	}

	// Set the pixels that corespond to the Mouse position
	//
	void AdjustRope( Point mousepos )
	{
		UpdateCanvas uc( this );

		SmartWin::Pen pen( uc, itsColor, itsDrawWidth );

		size_t histsize = itsHist.size();
		if ( histsize > 0 )
		{
			uc.line( itsHist.back(), mousepos );
		}
		itsHist.push( mousepos );

		if ( histsize > itsHist_max )
		{
			SmartWin::Pen pen( uc, itsBackgroundColor, itsEraseWidth );
			SmartWin::Point discard = itsHist.front();
			itsHist.pop();

			uc.line( discard, itsHist.front() );
		}
	}

	//  IN: virtkey is a virtual key, which includes the special keys.
	bool KeyPressed( int virtkey )
	{
		switch ( virtkey )
		{   // Arrows
			case VK_LEFT : itsColor = RandColor();
				return true;

			case VK_UP : itsDrawWidth++;
				itsEraseWidth++;
				return true;

			case VK_RIGHT : itsColor = RandColor();
				return true;

			case VK_DOWN : itsDrawWidth--;
				itsEraseWidth--;
				return true;
			default: return false;
		}

		return true;
	}

	void heartbeatResponse( const CommandPtr & cmd )
	{
		if ( itsHist.size() > 0 )
			AdjustRope( itsHist.back() );
		Command cmd2( _T( "Testing" ) ); // Must do it like this since due to private Constructor gcc complains if created within actual function call
		createTimer( & Encircle::heartbeatResponse, heartbeat, cmd2 );
	}
};
