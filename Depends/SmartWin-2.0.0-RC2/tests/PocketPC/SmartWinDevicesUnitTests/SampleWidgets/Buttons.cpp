// $Revision: 1.18 $
/* Copyright (c) 2005 - Thomas Hansen
 * Part of the SmartWin++ library example code
 * Permission to use under the same terms as the actual library
 * The license to the library can be found at http://smartwinlib.org
 * License for library is Open Source and BSD
 */
#include "Buttons.h"
#include "TabSheetWidget.h"
#include "resource.h"

using namespace SmartWin;

Buttons::Buttons( TabSheetWidget * parent )
		: Widget( parent ),
		sw::WidgetFactory< sw::WidgetChildWindow, Buttons >( parent ),
		itsParent( parent ),
		itsBgBitmap( new Bitmap( IDB_BITMAP1 ) )
{}

void Buttons::clicked( WidgetButtonPtr btn )
{
	::MessageBeep( MB_ICONQUESTION );
	itsStatic->setText( btn->getText() + _T( " clicked" ) );
}

void Buttons::dblClicked( WidgetButtonPtr btn )
{
	::MessageBeep( MB_ICONQUESTION );
	itsStatic->setText( btn->getText() + _T( " DOUBLE clicked" ) );
}

void Buttons::paintBitmapButton( WidgetButtonPtr btn, Canvas & canvas )
{
	Brush bitmapBrush( canvas, itsBgBitmap );
	sw::Rectangle rc( btn->getClientAreaSize() );
	canvas.fillRectangle( rc, bitmapBrush );
	canvas.setTextColor( RGB( 11, 0, 11 ) );
	canvas.setBkMode( true );
	boost::shared_ptr< Font > font( btn->getFont() );
	canvas.selectFont( font );
	canvas.drawText( btn->getText(), rc, DT_CENTER | DT_VCENTER );
}

void Buttons::layout()
{
	// Resizing main client area
	sw::Rectangle rc( itsParent->getClientAreaSize() );
	setBounds( 10, 30, rc.size.x - 20, rc.size.y - 40 );

	itsButton->setBounds( rc.pos.x + 15, 35, 55, 35 );
	itsImgButton->setBounds( rc.pos.x + 95, 35, 55, 35 );
	itsStatic->setBounds( rc.pos.x + 5, 75, 160, 80 );
	itsCheckBox->setBounds( rc.pos.x + 5, 160, 160, 16 );
}

void Buttons::initAndCreate()
{
	// Creating main DataGrid container window
	createWindow();

	// Creating bgColor button...
	itsButton = createButton();
	itsButton->setText( _T( "Piip!" ) );
	itsButton->onClicked( & Buttons::clicked );

	// Creating CheckBox
	itsCheckBox = createCheckBox();
	itsCheckBox->setText( _T( "Check me" ) );

	// Creating bgImageButton
	WidgetButton::Seed buttonCS;

	buttonCS.style |= BS_OWNERDRAW;
	buttonCS.caption = _T( "Piip II!" );
	buttonCS.font = createFont( _T( "Comic Sans" ), 28, 8, 4, ANSI_CHARSET, false, false, false, 0, OUT_DEFAULT_PRECIS );
	itsImgButton = createButton( buttonCS );
	itsImgButton->onClicked( & Buttons::clicked );
	itsImgButton->onDblClicked( & Buttons::dblClicked );
	itsImgButton->onPainting( & Buttons::paintBitmapButton );

	// Creating Static Control
	itsStatic = createStatic();
	itsStatic->setText( _T( "Try to click any of the above buttons (yes, the above green stuff is really an owner drawn button)" ) );

	// Once And Once Only
	layout();
}
