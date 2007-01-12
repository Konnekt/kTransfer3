// $Revision: 1.9 $
/* Copyright (c) 2005 - Thomas Hansen
 * Part of the SmartWin++ library example code
 * Permission to use under the same terms as the actual library
 * The license to the library can be found at http://smartwinlib.org
 * License for library is Open Source and BSD
 */ 
/*  A demonstration of the use of SmartWin::PaintCanvas and UpdateCanvas.

	PaintCanvas is in the reponse function for  onPainting().
	UpdateCanvas may be used in other times.  Naturally, you want the results of
	UpdateCanvas to be preserved when the window is resized or uncovered.

	Fundamentally, the functions from the Windows GDI are used. You can either use
	HDC getDc() to get a HDC and then call the windows GDI functions directly, or
	just use the wrapping GDI functions built into CanvasClasses.h

	There is the GDI+ alternative to SmartWin drawing functions if the target is
	modern Windows. Remove the comment on UseGdiPlus to enable this.
 */
#include "SmartWin.h"
using namespace SmartWin;
#include "encircle.h"

// Remove the comment part below to use GdiPlus and view an Image in addition to the other cool stuff you see... ;)
//#define UseGdiPlus 1
#ifdef UseGdiPlus
#include <gdiplus.h>
using namespace Gdiplus;
#endif

class CanvasWidget
			: public WidgetFactory< WidgetWindow, CanvasWidget >
{
	//WidgetMenuPtr ptrSysMenu;
	WidgetMenuPtr ptrMainMenu;

public:
	void init()
	{
		createWindow();

		setText( _T( "Samples for PaintCanvas, UpdateCanvas, Mouse and Keyboard events" ) );
		onSized( & CanvasWidget::isResized );

		WidgetChooseColor dlg = createChooseColor();
		WidgetChooseColor::ColorParams params = dlg.showDialog( false ); // Initial color
		if ( params.userPressedOk() )
		{
			itsColor = params.getColor();
		}
		else
		{
			itsColor = 0x0000FFFF; // Defaulting to yellow...
		}

		onPainting( & CanvasWidget::painting );
		updateWidget();

		onLeftMouseDown( & CanvasWidget::PixelColorMousePosition );
		onRightMouseDown( & CanvasWidget::PixelColorMousePosition );
		onMiddleMouseDown( & CanvasWidget::PixelColorMousePosition );
		onMouseMove( & CanvasWidget::MouseMoveResponse );
		onKeyPressed( & CanvasWidget::KeyPressed );

		ptrMainMenu = createMenu();
		//ptrSysMenu = ptrMainMenu->getSystemMenu();
		//ptrSysMenu->appendItem( 1, _T("&Choose New Color..."), &CanvasWidget::getColor );
	}

	void getColor( WidgetMenuPtr menuItem, unsigned item )
	{
		WidgetChooseColor dlg = createChooseColor();
		WidgetChooseColor::ColorParams color = dlg.showDialog( true );
		if ( color.userPressedOk() )
		{
			itsColor = color.getColor();
			updateWidget();
		}
	}

	// Only if GDI+ is in use...
#ifdef UseGdiPlus
	// GdiPlus requires a token that is created at startup, and
	// used to identify the release.
	ULONG_PTR itsGdiplusToken;

	CanvasWidget()
	{
		// Initialize GDI+.
		GdiplusStartupInput gdiplusStartupInput;
		GdiplusStartup( & itsGdiplusToken, & gdiplusStartupInput, NULL );
	}

	~CanvasWidget()
	{
		GdiplusShutdown( itsGdiplusToken );
	}
#endif

private:
	COLORREF itsColor;

	COLORREF RandColor()
	{
		return ( RGB( rand() % 255, rand() % 255, rand() % 255 ) );
	}

	void painting( SmartWin::Canvas & c )
	{
		// rc.pos is the upper left point, rc.size is the lower right point.
		SmartWin::Rectangle rc( getClientAreaSize() );
		RECT kokko = rc;

		// Divide the dimensions into halves, quarters and eighths
		int x1 = rc.size.x, x2 = x1 / 2, x4 = x2 / 2, x8 = x4 / 2,
			y1 = rc.size.y, y2 = y1 / 2, y4 = y2 / 2, y8 = y4 / 2;

		// Use this color until pen and brush goes out of scope.
		SmartWin::Pen pen( c, itsColor );
		SmartWin::Brush brush( c, itsColor );

		// Use the existing background for text

		c.setBkColor( c.getPixel( 0, y2 ) );

		// Do a number of Graphics operations, and show the calls
		//
#ifdef UseGdiPlus
		int items = 9;
#else
		int items = 8;
#endif
		int spacing = 10;
		int yinc = ( y1 - ( items * spacing ) ) / items;
		if ( yinc < 1 )
		{
			SmartWin::TextPen textpen( c, itsColor ); // Applies only in block.
			std::basic_stringstream< TCHAR > ss;
			ss << _T( "Please make me more than " ) << x1 << _T( "," ) << y1;
			MessageAtLeftTop( SmartUtil::tstring( ss.str() ) );
			return; // Not enough room for the rest of the graphics.
		}

		int x = 0;
		int y = 0;
		c.extTextOut( _T( " void painting( SmartWin::PaintCanvas & c ) " ), x2, y );
		y += yinc + spacing;

		// Scoping out the TextPen object to have RAII on the pen
		// Basically what's happening here is that the pen will be selected for the canvas when
		// the pen is created and when it's destroyed (at the end of the scope) the pen will
		// be released
		{
			SmartWin::TextPen textpen( c, itsColor );
			c.extTextOut( _T( " Greetings human ! " ), x, y );
		}
		c.extTextOut( _T( " c.extTextOut( \" Greetings human ! \", x, y ); " ), x2, y );
		y += yinc + spacing;

		c.rectangle( x, y, x2, y + yinc );
		c.extTextOut( _T( " c.Rectangle( x,y, x2, y+yinc ) " ), x2, y );
		y += yinc + spacing;

		c.ellipse( x, y, x2, y + yinc );
		c.extTextOut( _T( " c.ellipse( x,y, x2, y+yinc ) " ), x2, y );
		y += yinc + spacing;

		// Creating and drawing our polygon
		POINT polypoints[] =
		{
								 { x4 - x8, y
								 },
								 { x4 + x8, y
								 },
								 { x2, y + ( yinc / 2 )
								 },
								 { x4 + x8, y + yinc
								 },
								 { x4 - x8, y + yinc
								 },
								 { 0, y + ( yinc / 2 )
								 }
							 };
		c.polygon( polypoints, sizeof( polypoints ) / sizeof( POINT ) );

		c.extTextOut( _T( " c.polygon( polypoints, sizeof(polypoints)/sizeof(POINT) ); " ), x2, y );
		y += yinc + spacing;

		c.extTextOut( _T( " c.setPixel( x, y, itsColor );" ), x2, y );
		for ( x = x2; x > 0; x-- )
			c.setPixel( x, y + ( rand() % yinc ), itsColor );

		y += yinc + spacing;

		// Draw a triangle
		c.extTextOut( _T( " c.moveTo( 0, y );             c.lineTo( x2, y );" ), x2, y );
		c.extTextOut( _T( " c.line( x2, y, x4, y+yinc );  c.lineTo( 0, y );" ), x2, y + ( yinc / 2 ) );

		c.moveTo( 0, y );
		c.lineTo( x2, y );
		c.line( x2, y, x4, y + yinc );
		c.lineTo( 0, y );
		y += yinc + spacing;

		// Draw the same triangle, and then fill it with our color.
		c.line( 0, y, x2, y );
		c.lineTo( x4, y + yinc );
		c.lineTo( 0, y );

		COLORREF color = c.getPixel( x4, y + 5 );
		c.extTextOut( _T( " c.extFloodFill( x4, y+5, color, false );" ), x2, y );
		c.extFloodFill( x4, y + 5, color, false ); // Fill while we find the color.  FLOODFILLSURFACE
		y += yinc + spacing;

#ifdef UseGdiPlus
		//  Draw a JPG file using Window's GDI classes.
		GdiPlus( c.getDc(), 0, y, x2, y + yinc ); // GDI+ Logic
#endif
	}

#ifdef UseGdiPlus
	//  Draws a diagonal line and a JPG image
	void GdiPlus( HDC hdc, int x, int y, int xmax, int ymax )
	{
		Graphics graphics( hdc );
		Gdiplus::Pen pen( Color( 255, 0, 0, 255 ) ); // Alpha, Red, green, Blue
		graphics.DrawLine( & pen, x, y, xmax, ymax );

		Image image( L"sample.jpg" );

		graphics.DrawImage( & image, x, y );
	}
#endif
	// There are new dimensions to the window because the user resized the window.
	void isResized( const SmartWin::WidgetSizedEventResult & sz )
	{
		updateWidget();
	}

	//  Respond to a mouse click by setting the pixel to a random color
	//  and displaying a text message
	void PixelColorMousePosition( const SmartWin::MouseEventResult & mouse )
	{
		UpdateCanvas uc( this );
		uc.setPixel( mouse.pos.x, mouse.pos.y, RandColor() );

		SmartUtil::tstring msg;
		switch ( mouse.ButtonPressed )
		{
			case MouseEventResult::LEFT : msg = _T( "<- Left button" ); break;
			case MouseEventResult::MIDDLE : msg = _T( "<- Middle button" ); break;
			case MouseEventResult::RIGHT : msg = _T( "<- Right button" ); break;
			case MouseEventResult::OTHER : msg = _T( "<- Other button" ); break;
		}
		if ( mouse.isShiftPressed ) msg += _T( " shifted" );
		if ( mouse.isAltPressed ) msg += _T( " alted" );
		if ( mouse.isControlPressed ) msg += _T( " controlled" );
		uc.extTextOut( msg, mouse.pos.x + 10, mouse.pos.y );
	}

	// Respond to Mouse movement by showing "Mouse moved to x,y"
	void MouseMoveResponse( const SmartWin::MouseEventResult & mouse )
	{
		std::basic_stringstream< TCHAR > ss;
		ss << _T( "Mouse moved to " ) << mouse.pos.x << _T( "," ) << mouse.pos.y;
		if ( mouse.isShiftPressed ) ss << _T( " shifted" );
		MessageAtLeftTop( SmartUtil::tstring( ss.str() ) );
	}

	// We are sending this message outside of a onPainting response function.
	// So we are using an UpdateCanvas
	void MessageAtLeftTop( const SmartUtil::tstring & msg )
	{
		UpdateCanvas uc( this );
		SmartWin::Rectangle r( 0, 0, 256, 40 );
		uc.rectangle( r );
		uc.drawText( msg, r, DT_SINGLELINE | DT_CENTER | DT_VCENTER );
	}

	//  IN: virtkey is a virtual key, which includes the special keys.
	bool KeyPressed( int virtkey )
	{
		SmartUtil::tstring msg;

		switch ( virtkey )
		{
			case VK_RETURN : msg = _T( "VK_RETURN" ); break;
			case VK_ESCAPE : msg = _T( "VK_ESCAPE" );
				{
					Encircle * cw = new Encircle(); cw->init();
				}
				break;
			case VK_SPACE : msg = _T( "VK_SPACE" ); break;
			case VK_CONTROL : msg = _T( "VK_CONTROL" ); break;
			case VK_SHIFT : return false;
			case VK_NEXT : msg = _T( "VK_NEXT" ); break; // Page Down
			case VK_PRIOR : msg = _T( "VK_PRIOR" ); break; // Page Up
			case VK_INSERT : msg = _T( "VK_INSERT" ); break;
			case VK_DELETE : msg = _T( "VK_DELETE" ); break;
			case VK_END : msg = _T( "VK_END" ); break;
			case VK_HOME : msg = _T( "VK_HOME" ); break;
			case VK_LEFT : msg = _T( "VK_LEFT" ); break; // Arrows
			case VK_UP : msg = _T( "VK_UP" ); break;
			case VK_RIGHT : msg = _T( "VK_RIGHT" ); break;
			case VK_DOWN : msg = _T( "VK_DOWN" ); break;
			case VK_F1 : msg = _T( "VK_F1" ); break; // Function Keys
			case VK_F2 : msg = _T( "VK_F2" ); break;
			case VK_F3 : msg = _T( "VK_F3" ); break;
			case VK_F4 : msg = _T( "VK_F4" ); break;
			case VK_F5 : msg = _T( "VK_F5" ); break;
			case VK_F6 : msg = _T( "VK_F6" ); break;
			case VK_F7 : msg = _T( "VK_F7" ); break;
			case VK_F8 : msg = _T( "VK_F8" ); break;
			case VK_F9 : msg = _T( "VK_F9" ); break;
			case VK_F10 : msg = _T( "VK_F10" ); break;
			case VK_F11 : msg = _T( "VK_F11" ); break;
			case VK_F12 : msg = _T( "VK_F12" ); break;

			default:
				bool control = getControlPressed();
				char key = virtualKeyToChar( virtkey );
				if ( key != 0 )
				{
					msg.append( _T( "Key '" ) );
					msg.append( & key, 1 );
					msg.append( _T( "' was pressed" ) );
					if ( control )
						msg.append( _T( " + control key." ) );
					else
						msg.append( _T( "." ) );
				}
				else
				{
					return false;
				}
		}
		MessageAtLeftTop( SmartUtil::tstring( msg ) );
		return true;
	}
};

// Application entry point just like all other SmartWin applications...
int SmartWinMain( Application & app )
{
	CanvasWidget * cw = new CanvasWidget(); cw->init();
	return ( app.run() );
}
