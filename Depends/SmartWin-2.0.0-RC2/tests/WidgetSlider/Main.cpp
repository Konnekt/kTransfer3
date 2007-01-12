// $Revision: 1.14 $
// Unit test for WidgetTextBox, tries to instantiate all different forms of WidgetTextBox and also tries to use all functions

#include "SmartWin.h"
#include "resource.h"
#include <sstream>
using namespace SmartWin;

// The "factory" version
class WidgetTest1
	: public WidgetFactory< WidgetWindow, WidgetTest1 >
{
	WidgetStatusBarPtr status;
public:

	WidgetTest1()
	{}

	void scrolling( WidgetSliderPtr slider )
	{
		status->setText( boost::lexical_cast< SmartUtil::tstring >( slider->getPosition() ) );
	}

	void resized( const WidgetSizedEventResult & sz )
	{
		status->refresh();
	}

	bool keyPressed( WidgetSliderPtr slider, int key )
	{
		this->setText( this->getText() + ( TCHAR ) virtualKeyToChar( key ) );
		return true;
	}

	void mouseMoved( WidgetSliderPtr slider, const MouseEventResult & res )
	{
		SmartUtil::tstring tmp = _T( "Mouse moved: " );
		tmp += boost::lexical_cast< SmartUtil::tstring >( res.pos.x );
		tmp += _T( ", " );
		tmp += boost::lexical_cast< SmartUtil::tstring >( res.pos.y );
		this->setText( tmp );
	}

	BitmapPtr itsBitmap;

	void painting( WidgetSliderPtr slider, Canvas & canvas )
	{
		// Retrieving size...
		sw::Rectangle rc( slider->getClientAreaSize() );

		// Bitmap first
		Brush brush( canvas, itsBitmap );
		canvas.fillRectangle( sw::Rectangle( slider->getSize() ), brush );

		// Then text
		canvas.setTextColor( RGB( 0, 128, 0 ) );
		canvas.setBkMode( true );
		canvas.drawText( _T( "Girlfriend slider" ), sw::Rectangle( 0, 0, rc.size.x, 20 ), 0 );

		// Then math behind thumb of slider
		int max = slider->getMaxValue();
		int min = slider->getMinValue();
		int range = max - min;
		int pos = slider->getPosition();
		pos += abs( min );

		double percentage = ( double ) pos / ( double ) range;
		rc.pos.x = ( long )( rc.size.x * percentage );
		rc.size.x = 5;
		for ( int x = rc.pos.x; x < rc.pos.x + rc.size.x; ++x )
		{
			for ( int y = rc.pos.y; y < rc.pos.y + rc.size.y; ++y )
			{
				COLORREF color = ColorUtilities::alphaBlend( canvas.getPixel( x, y ), RGB( std::min( 255, x * 2 ), 255 - std::min( 255, y ), std::min( 255, y ) ) );
				canvas.setPixel( x, y, color );
			}
		}
	}

	bool mouseIsDown;

	void mouseMove( WidgetSliderPtr slider, const MouseEventResult & res )
	{
		if ( mouseIsDown && res.pos.x > 0 )
		{
			sw::Rectangle rc( slider->getClientAreaSize() );
			if ( res.pos.x < rc.size.x )
			{
				int max = slider->getMaxValue();
				int min = slider->getMinValue();
				int range = max - min;
				double percentage = ( double ) res.pos.x / ( double ) rc.size.x;
				int position = ( int )( range * percentage );
				slider->setPosition( position - abs( min ) );
				slider->updateWidget();
			}
		}

		// Faking our scroll message
		scrolling( slider );
	}

	void mouseDown( WidgetSliderPtr slider, const MouseEventResult & res )
	{
		mouseIsDown = true;
		::SetCapture( slider->handle() );
		sw::Rectangle rc( slider->getClientAreaSize() );
		if ( res.pos.x < rc.size.x )
		{
			int max = slider->getMaxValue();
			int min = slider->getMinValue();
			int range = max - min;
			double percentage = ( double ) res.pos.x / ( double ) rc.size.x;
			int position = ( int )( range * percentage );
			slider->setPosition( position - abs( min ) );
			slider->updateWidget();
		}

		// Faking our scroll message
		scrolling( slider );
	}

	void mouseUp( WidgetSliderPtr slider, const MouseEventResult & res )
	{
		mouseIsDown = false;
		::ReleaseCapture();
	}

	void initAndCreate()
	{
		mouseIsDown = false;
		WidgetWindow::Seed windowCS;

		windowCS.caption = _T( "WidgetSlider compile and execute test" );
		windowCS.background = CreateSolidBrush( RGB( 0, 0, 125 ) );
		windowCS.location = SmartWin::Rectangle( 100, 100, 600, 600 );
		createWindow( windowCS );

		WidgetLoadFile file = createLoadFile();
		file.addFilter( _T( "Bitmaps" ), _T( "BMP" ) );
		SmartUtil::tstring path = file.showDialog();
		if ( path != _T( "" ) )
			itsBitmap = BitmapPtr( new Bitmap( path ) );
		else
			itsBitmap = BitmapPtr( new Bitmap( IDB_BITMAP1 ) );

		status = createStatusBar();

		onSized( & WidgetTest1::resized );

		// Default a slider is created with vertically scrolling
		WidgetSliderPtr slider = createSlider();
		slider->setAutoTicks( true );
		slider->setHorizontal( false );
		slider->setShowTicksBoth( true );
		slider->setBounds( 100, 100, 50, 200 );
		slider->setRange( - 20, 20 );
		slider->setPosition( - 20 );
		slider->setTickFrequency( 5 );
		slider->onScrollVert( & WidgetTest1::scrolling );
		slider->onKeyPressed( & WidgetTest1::keyPressed );

		slider = createSlider();
		slider->setAutoTicks( true );
		slider->setHorizontal( false );
		slider->setShowTicksLeft( false );
		slider->setBounds( 10, 100, 50, 200 );
		slider->setRange( - 20, 20 );
		slider->setPosition( 10 );
		slider->onScrollVert( & WidgetTest1::scrolling );
		slider->onMouseMove( & WidgetTest1::mouseMoved );

		slider = createSlider();
		slider->setAutoTicks( true );
		slider->setHorizontal( true );
		slider->setShowTicksTop( true );
		slider->setBounds( 250, 300, 154, 205 );
		slider->setRange( 0, 100 );
		slider->setPosition( 20 );
		slider->onScrollHorz( & WidgetTest1::scrolling );
		slider->onPainting( & WidgetTest1::painting );
		slider->onMouseMove( & WidgetTest1::mouseMove );
		slider->onLeftMouseDown( & WidgetTest1::mouseDown );
		slider->onLeftMouseUp( & WidgetTest1::mouseUp );

		// Buddy control...
		WidgetTextBoxPtr box = createTextBox();
		box->setBounds( 150, 30, 70, 20 );
		box->setEnabled( false );
		box->setText( _T( "Minimum" ) );
		slider->assignBuddy( true, box );
		box = createTextBox();
		box->setText( _T( "Maximum" ) );
		box->setEnabled( false );
		box->setBounds( 150, 30, 70, 20 ); // We don't really care about the position of the edit box since it'll be moved anyway...
		slider->assignBuddy( false, box );

		// Note!
		// Since this control is set to not enabled there is no way we can manipulate it's value...
		slider = createSlider();
		slider->setAutoTicks( true );
		slider->setHorizontal( true );
		slider->setShowTicksTop( false );
		slider->setBounds( 150, 180, 200, 50 );
		slider->setRange( - 10, 10 );
		slider->setEnabled( false );
		assert( slider->getPosition() == 0 );
		slider->onScrollHorz( & WidgetTest1::scrolling );
	}
};

int SmartWinMain( Application & app )
{
	// Widget (Factory) version
	WidgetTest1 * testWnd1 = new WidgetTest1;
	testWnd1->initAndCreate();
	return app.run();
}
