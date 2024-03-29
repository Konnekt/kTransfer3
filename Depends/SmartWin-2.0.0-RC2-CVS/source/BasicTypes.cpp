// $Revision$
/*
  Copyright (c) 2005, Thomas Hansen
  All rights reserved.

  Redistribution and use in source and binary forms, with or without modification,
  are permitted provided that the following conditions are met:

	  * Redistributions of source code must retain the above copyright notice,
		this list of conditions and the following disclaimer.
	  * Redistributions in binary form must reproduce the above copyright notice,
		this list of conditions and the following disclaimer in the documentation
		and/or other materials provided with the distribution.
	  * Neither the name of the SmartWin++ nor the names of its contributors
		may be used to endorse or promote products derived from this software
		without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
  INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#include "../include/smartwin/BasicTypes.h"

namespace SmartWin
{
// begin namespace SmartWin

Point::Point( long pX, long pY )
	: x( pX ), y( pY )
{}

Point::Point()
	: x( 0 ), y( 0 )
{}

void Point::maxOf( const Point & p )
{
	if ( p.x > x )
		x = p.x;
	if ( p.y > y )
		y = p.y;
}

void Point::minOf( const Point & p )
{
	if ( p.x < x )
		x = p.x;
	if ( p.y < y )
		y = p.y;
}

Point & operator += ( Point & lhs, const Point & rhs )
{
	lhs.x += rhs.x;
	lhs.y += rhs.y;
	return lhs;
}

Point operator +( const Point & lhs, const Point & rhs )
{
	Point retVal = lhs;
	retVal += rhs;
	return retVal;
}

Point & operator -= ( Point & lhs, const Point & rhs )
{
	lhs.x -= rhs.x;
	lhs.y -= rhs.y;
	return lhs;
}

Point operator -( const Point & lhs, const Point & rhs )
{
	Point retVal = lhs;
	retVal -= rhs;
	return retVal;
}

bool operator == ( const Point & lhs, const Point & rhs )
{
	return lhs.x == rhs.x && lhs.y == rhs.y;
}

bool operator != ( const Point & lhs, const Point & rhs )
{
	return !( lhs == rhs );
}

Rectangle::Rectangle()
	: pos( Point() ), size( Point() )
{}

Rectangle::Rectangle( const Point & pPos, const Point & pSize )
	: pos( pPos ), size( pSize )
{}

Rectangle::Rectangle( const Point & pSize )
	: size( pSize )
{}

Rectangle::Rectangle( long x, long y, long width, long height )
	: pos( x, y ), size( width, height )
{}

Rectangle::Rectangle( const Rectangle & rect,
			double xFraction, double yFraction,
			double widthFraction, double heightFraction )
	: pos( rect.pos ), size( rect.size )
{
	* this = subRect( xFraction, yFraction, widthFraction, heightFraction );
}

Rectangle::operator const ::RECT() const
{
	RECT retVal;
	retVal.left = pos.x;
	retVal.top = pos.y;

	// TODO: Is this right??
	// (Adding pos to size to get bottom/right...)
	retVal.right = pos.x + size.x;
	retVal.bottom = pos.y + size.y;
	return retVal;
}

Rectangle Rectangle::FromRECT( const ::RECT & a_RECT )
{
	return Rectangle( a_RECT.left, a_RECT.top, a_RECT.right - a_RECT.left, a_RECT.bottom - a_RECT.top );
}

Point Rectangle::lowRight() const
{
	return Point( pos.x + size.x, pos.y + size.y );
}

Rectangle Rectangle::subRect( double xFraction, double yFraction,
				double widthFraction, double heightFraction ) const
{
	Rectangle retVal;
	retVal.pos.x = pos.x + ( long )( xFraction * size.x );
	retVal.pos.y = pos.y + ( long )( yFraction * size.y );

	retVal.size.x = ( long )( size.x * widthFraction );
	retVal.size.y = ( long )( size.y * heightFraction );
	return retVal;
}

// Size of the rectangle will be * factor.
// Position adjusted for the same center.
Rectangle Rectangle::shrink( double factor ) const
{
	double posFactor = ( 1.0 - factor ) * 0.5;
	return subRect( posFactor, posFactor, factor, factor );
}

Rectangle Rectangle::shrinkWidth( long xBorder ) const
{
	Rectangle retVal = * this;
	retVal.pos.x = pos.x + xBorder;
	retVal.size.x = size.x - ( xBorder + xBorder );
	return retVal;
}

Rectangle Rectangle::shrinkHeight( long yBorder ) const
{
	Rectangle retVal = * this;
	retVal.pos.y += yBorder;
	retVal.size.y -= ( yBorder + yBorder );
	return retVal;
}

Rectangle Rectangle::shrink( long border ) const
{
	Rectangle retVal = shrinkWidth( border );
	retVal = retVal.shrinkHeight( border );
	return retVal;
}

Rectangle Rectangle::shrink( long xBorder, long yBorder ) const
{
	Rectangle retVal = shrinkWidth( xBorder );
	retVal = retVal.shrinkHeight( yBorder );
	return retVal;
}

Rectangle Rectangle::getTop( long y ) const
{
	Rectangle top = Rectangle( pos, Point( size.x, y ) );
	return top;
}

Rectangle Rectangle::getBottom( long y ) const
{
	Rectangle bottom = Rectangle(
		Point( pos.x, pos.y + size.y - y ),
		Point( size.x, y ) );
	return bottom;
}

Rectangle Rectangle::getLeft( long x ) const
{
	Rectangle left = Rectangle( pos, Point( x, size.y ) );
	return left;
}

Rectangle Rectangle::getRight( long x ) const
{
	Rectangle bottom = Rectangle(
		Point( pos.x + size.x - x, pos.y ),
		Point( x, size.y ) );
	return bottom;
}

Rectangle Rectangle::upperLeftAdjust( const Point & adjust ) const
{
	Rectangle retVal = * this;
	retVal.pos.x += adjust.x;
	retVal.pos.y += adjust.y;
	retVal.size.x -= adjust.x;
	retVal.size.y -= adjust.y;
	return retVal;
}

Rectangle Rectangle::lowerRightAdjust( const Point & adjust ) const
{
	Rectangle retVal = * this;
	retVal.size.x += adjust.x;
	retVal.size.y += adjust.y;
	return retVal;
}

Rectangle Rectangle::left( double portion ) const
{
	return Rectangle( pos.x, pos.y, ( long ) ( size.x * portion ), size.y );
}

Rectangle Rectangle::right( double portion ) const
{
	return Rectangle( pos.x + ( long ) ( ( 1.0 - portion ) * size.x ), pos.y,
					   ( long ) ( size.x * portion ), size.y );
}

Rectangle Rectangle::top( double portion ) const
{
	return Rectangle( pos.x, pos.y, size.x, ( long ) ( size.y * portion ) );
}

Rectangle Rectangle::bottom( double portion ) const
{
	return Rectangle( pos.x, pos.y + ( long ) ( ( 1.0 - portion ) * size.y ),
			size.x, ( long ) ( size.y * portion ) );
}

Rectangle Rectangle::row( int row, int rows ) const
{
	int rowheight = size.y / rows;
	return Rectangle( pos.x, pos.y + ( row * rowheight ), size.x, rowheight );
}

Rectangle Rectangle::col( int column, int columns ) const
{
	int colwidth = size.x / columns;
	return( Rectangle( pos.x + ( colwidth * column ), pos.y, colwidth, size.y ) );
}

Rectangle Rectangle::cropTop( const int a_ToRemove ) const
{
	register int d_NewSize = size.y - a_ToRemove;

	return Rectangle( pos.x, pos.y + a_ToRemove, size.x, d_NewSize > 0 ? d_NewSize : 0 );
}

Rectangle Rectangle::cropBottom( const int a_ToRemove ) const
{
	register int d_NewSize = size.y - a_ToRemove;

	return Rectangle( pos.x, pos.y, size.x, d_NewSize > 0 ? d_NewSize : 0 );
}

Rectangle Rectangle::cropLeft( const int a_ToRemove ) const
{
	register int d_NewSize = size.x - a_ToRemove;

	return Rectangle( pos.x + a_ToRemove, pos.y, d_NewSize > 0 ? d_NewSize : 0, size.y );
}

Rectangle Rectangle::cropRight( const int a_ToRemove ) const
{
	register int d_NewSize = size.x - a_ToRemove;

	return Rectangle( pos.x, pos.y, d_NewSize > 0 ? d_NewSize : 0, size.y );
}

const SmartWin::Rectangle letTheSystemDecide( CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT );

const DontInitialize DontInitializeMe;

// end namespace SmartWin
}
