// $Revision: 1.16 $
/* Copyright (c) 2005 - Thomas Hansen
 * Part of the SmartWin++ library example code
 * Permission to use under the same terms as the actual library
 * The license to the library can be found at http://smartwinlib.org
 * License for library is Open Source and BSD
 */
#include "Sliders.h"
#include "TabSheetWidget.h"
#include "resource.h"

using namespace SmartWin;

Sliders::Sliders( TabSheetWidget * parent )
		: Widget( parent ),
		sw::WidgetFactory< sw::WidgetChildWindow, Sliders >( parent ),
		itsParent( parent ),
		itsBitmap( new Bitmap( IDB_BITMAP4 ) )
{}

void Sliders::sliderMoved( WidgetSliderPtr slider )
{
	SmartUtil::tstring value = boost::lexical_cast< SmartUtil::tstring >( slider->getPosition() );
	itsSliderBuddy->setText( value );
	itsProgress->setPosition( slider->getPosition() * 100 );
}

// Mark, here we're showing use of a global event handler just for the fun of it!
// As you see here we get the Sliders pointer explicitly given instead of the way we would have gotten
// it in a "this" pointer if it was a Sliders member function!
void painting( Sliders * This, Canvas & canvas )
{
	Brush brush( canvas, This->itsBitmap );
	sw::Rectangle rc( This->getClientAreaSize() );
	canvas.fillRectangle( rc, brush );
}

BrushPtr Sliders::backgroundColor( WidgetTextBoxPtr slider, Canvas & canvas )
{
	canvas.setBkMode( true );
	itsBgBrush = BrushPtr( new Brush( canvas, RGB( 255, 0, 128 ) ) );
	return itsBgBrush;
}

void Sliders::initAndCreate()
{
	createWindow();

	// Creating Widgets
	itsSpinnerBuddy = createTextBox();
	itsSpinnerBuddy->onBackgroundColor( & Sliders::backgroundColor );

	itsSliderBuddy = createTextBox();
	itsSliderBuddy->onBackgroundColor( & Sliders::backgroundColor );

	WidgetSpinner::Seed spinnerCS;

	spinnerCS.style = UDS_SETBUDDYINT | WS_VISIBLE;
	spinnerCS.minValue = - 10;
	spinnerCS.maxValue = 10;
	itsSpinner = createSpinner( spinnerCS );
	itsSpinner->assignBuddy( itsSpinnerBuddy );

	itsSlider = createSlider();
	itsSlider->setAutoTicks( true );
	itsSlider->setHorizontal( true );
	itsSlider->setShowTicksTop( false );
	itsSlider->setBounds( 10, 100, 50, 200 );
	itsSlider->setRange( - 10, 10 );
	itsSlider->setPosition( - 8 );
	itsSlider->onScrollHorz( & Sliders::sliderMoved );

	itsProgress = createProgressBar();
	itsProgress->setRange( - 1000, 1000 );
	itsProgress->setPosition( 100 );
	itsProgress->setStep( 100 );
	onPainting( & painting );

	layout();
}

void Sliders::layout()
{
	// Resizing main client area
	sw::Rectangle rc( itsParent->getClientAreaSize() );
	setBounds( 10, 30, rc.size.x - 20, rc.size.y - 40 );

	rc = sw::Rectangle( this->getClientAreaSize() ); // lazy...
	rc = rc.getTop( 60 );

	Place place;
	place.setBoundsBorders( rc, 3, 3 );

	itsSpinner->setSizeAsGridPerPlace( place, 2, 3 );
	itsSlider->setSizeAsGridPerPlace( place, 2, 3 );
	itsSliderBuddy->setSizeAsGridPerPlace( place, 3, 6 );
	itsProgress->setSizeAsGridPerPlace( place, 3, 2 );
	itsSpinnerBuddy->setSizeAsGridPerPlace( place, 3, 6 );
}
