// $Revision: 1.12 $
//#define COMCTRL_V6

#include <sstream>
#include "SmartWin.h"
using namespace SmartWin;

class WidgetTest1
	: public WidgetFactory< WidgetWindow, WidgetTest1 >
{
	WidgetStatusBarPtr status;
	WidgetProgressBarPtr progressbar;
	WidgetButtonPtr button;
	WidgetStaticPtr stat;
	WidgetSliderPtr slider;

public:
	WidgetTest1()
	{}

	void scrolling( WidgetSlider * slider )
	{
		int position = slider->getPosition();
		progressbar->setPosition( position );

		position = progressbar->getPosition();
		SmartUtil::tstringstream str;
		str << position;
		status->setText( str.str() );
	}

	void resized( const WidgetSizedEventResult & sz )
	{
		status->refresh();
	}

	void clicked( WidgetButtonPtr button )
	{
		int RangeMin = progressbar->getMinValue();
		int RangeMax = progressbar->getMaxValue();
		int Pos = progressbar->getPosition();
		int Step = progressbar->getStep();
		SmartUtil::tstringstream str;
		str << "Range:\nMinimum " << RangeMin << "\n";
		str << "Maximum " << RangeMax << "\n\n";
		str << "Stepsize " << Step << "\n\n";
		str << "Position " << Pos;
		stat->setText( str.str() );
	}

//  SetVertical works well if using Comctrl v6 but doesn't work if not
//  You have to decide horizontal/vertical at control Creation
#ifdef COMCTRL_V6
	void SetVertical( WidgetButtonPtr button )
	{
		static bool style = false;
		progressbar->setHorizontal( style );
		progressbar->setBounds( 100, style ? 350 : 100, style ? 300 : 30, style ? 30 : 300 );
		button->setText( style ? _T( "Vertical" ) : _T( "Horizontal" ) );
		style = !style;
		scrolling( slider );
	}
#endif
	void Step( WidgetButtonPtr button )
	{
		progressbar->addStep();
	}

	void Addpos( WidgetButtonPtr button )
	{
		progressbar->addToPosition( 200 );
	}

	void SetCustomPainting( WidgetButtonPtr button )
	{
		progressbar->onPainting( & WidgetTest1::paint );
		progressbar->updateWidget();
		button->setEnabled( false );
	}


	void mouseMoved( WidgetProgressBarPtr bar, const MouseEventResult & res )
	{
		SmartUtil::tstring tmp = _T( "Mouse moved: " );
		tmp += boost::lexical_cast< SmartUtil::tstring >( res.pos.x );
		this->setText( tmp );
	}

	void paint( WidgetProgressBarPtr bar, Canvas & canvas )
	{
		// For the fun of it we're rendering a top/down progressbar control here...
		// Meaning if it's got 10% only the top 10% parts of it is filled up...
		Brush brushChosen( canvas, RGB( 255, 0, 0 ) );
		Brush brushNotChosen( canvas, RGB( 0, 255, 0 ) );
		int pos = bar->getPosition();
		int min = bar->getMinValue();
		int max = bar->getMaxValue();
		int range = max - min;
		double percentage = ( double )( pos - min ) / ( double ) range;
		sw::Rectangle rc( bar->getClientAreaSize() );
		canvas.fillRectangle( rc, brushNotChosen );
		rc.size.y = ( int )( ( double ) rc.size.y * percentage );
		canvas.fillRectangle( rc, brushChosen );
	}

	void initAndCreate()
	{
		WidgetWindow::Seed windowCS;

		windowCS.caption = _T( "WidgetProgressbar compile and execute test" );
		windowCS.location = SmartWin::Rectangle( 0, 50, 475, 460 );
		createWindow( windowCS );
		onSized( & WidgetTest1::resized );

		status = createStatusBar();

		WidgetProgressBar::Seed progBarCS;

#ifndef COMCTRL_V6
		// Create a vertical Progressbar
		progBarCS.style |= PBS_VERTICAL;
#endif
		progBarCS.location = SmartWin::Rectangle( 100,
			( progBarCS.style & PBS_VERTICAL ) ? 100 : 350,
			( progBarCS.style & PBS_VERTICAL ) ? 30 : 300,
			( progBarCS.style & PBS_VERTICAL ) ? 300 : 30
			);
		progressbar = createProgressBar( progBarCS );
		progressbar->onMouseMove( & WidgetTest1::mouseMoved );
		progressbar->setRange( - 5000, 5000 );
		progressbar->setPosition( 100 );
		progressbar->setStep( 100 );

		// Default a slider is created with vertically scrolling
		slider = createSlider();
		slider->setAutoTicks( false );
		slider->setHorizontal( false );
		slider->setShowTicksLeft( false );
		slider->setBounds( 10, 10, 50, 200 );
		slider->setRange( - 5000, 5000 );
		slider->setPosition( 10 );
		slider->onScrollVert( & WidgetTest1::scrolling );

		WidgetButton::Seed buttonCS;

		buttonCS.location = SmartWin::Rectangle( 100, 10, 120, 50 );
		buttonCS.caption = _T( "&Push Me" );
		button = createButton( buttonCS );
		assert( button->getText() == _T( "&Push Me" ) );
		button->onClicked( & WidgetTest1::clicked );
		FontPtr font = createFont( _T( "Times New Roman" ), 0, 10, 1, ANSI_CHARSET, true, false, false, 0, OUT_DEFAULT_PRECIS );
		button->setFont( font );
		button->setVisible( true );
		stat = createStatic();
		stat->setBounds( 250, 10, 200, 50 );
		stat->setText( _T( "Testing, one, two, three, four..." ) );
#ifdef COMCTRL_V6
		button = createButton();
		button->setBounds( 100, 60, 120, 30 );
		button->setText( _T( "Vertical" ) );
		button->onClicked( & WidgetTest1::SetVertical );
		button->setVisible( true );
#endif
		button = createButton();
		button->setBounds( 140, 100, 80, 30 );
		button->setText( _T( "++" ) );
		button->onClicked( & WidgetTest1::Step );
		button->setVisible( true );

		button = createButton();
		button->setBounds( 140, 140, 80, 30 );
		button->setText( _T( "+=200" ) );
		button->onClicked( & WidgetTest1::Addpos );
		button->setVisible( true );

		button = createButton();
		button->setBounds( 250, 60, 120, 30 );
		button->setText( _T( "Custom painting" ) );
		button->onClicked( & WidgetTest1::SetCustomPainting );
		button->setVisible( true );
	}
};

int SmartWinMain( Application & app )
{
	WidgetTest1 * testWnd1 = new WidgetTest1;
	testWnd1->initAndCreate();
	return app.run();
}
