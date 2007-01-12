// $Revision: 1.15 $
#include "SmartWin.h"
#include <vector>
#include <assert.h>
using namespace SmartWin;

// This solution demonstrates some of the more "advanced" features of SmartWin,
// e.g. Application Supplied Policies/Aspects and template WidgetWindow classes...

class MySplitterPainting
{
	COLORREF itsColor;
	COLORREF itsColorBorder;

protected:
	MySplitterPainting()
		: itsColorBorder( RGB( 0, 0, 190 ) ),
		itsColor( RGB( 0, 0, 0 ) )
	{}

	// This is the ONE function you MUST supply if you're supposed to inject your
	// own splitter painting logic policy/Aspect. In addition you *** MUST *** have
	// a virtual DTOR since the Widget WILL be destroyed from a WidgetBase *
	// Note!
	// Inside this function we could have done basically what we wanted, e.g.
	// displayed a bitmap, drawed a vector image etc... The possibilities for the
	// WidgetSplitter Painting policy are endless...
	void paintSplitter( unsigned width, unsigned ySize, HWND handle )
	{
		PaintCanvas canvas( handle );
		{
			// Borders
			Pen pen( canvas, itsColorBorder );
			canvas.line( 0, 0, 0, ySize );
			canvas.line( width - 1, 0, width - 1, ySize );
		}
		{
			// Inside
			Pen pen( canvas, itsColor );
			for ( unsigned idx = 1;
				idx < width - 1;
				++idx )
			{
				canvas.line( idx, 0, idx, ySize );
			}
		}
	}

	void paintSplitterMoving( SmartWin::Canvas & canvas, unsigned cursorX, unsigned cursorY, const SmartWin::Rectangle & rect )
	{
		// Painting a "cross hair"
		canvas.line( cursorX, rect.pos.y, cursorX, rect.size.y );
		canvas.line( cursorX - 100, cursorY, cursorX + 100, cursorY );
	}

	// Virtual D'TOR MUST be supplied in policy class since Widget WILL GUARANTEED
	// be destroyed by a WidgetBase *
	virtual ~MySplitterPainting()
	{}
public:

	// Lets do a little bit of magic now...
	// Watch closely........
	void setMyCoolColor( COLORREF color, COLORREF border )
	{
		itsColor = color;
		itsColorBorder = border;
	}

	// Part of the magic!
	// Overridden from WidgetBase
	void updateColor()
	{
		setMyCoolColor( RGB( rand()%255, rand()%255, rand()%255 ), RGB( rand()%255, rand()%255, rand()%255 ) );
		Widget * This = dynamic_cast< Widget * >( this );
		This->updateWidget();
	}
};

// The "factory" version
// There are default TWO possible values of the template argument, one is
// SmartWin::SplitterThinPaint and the other is SmartWin::SplitterCoolPaint. If you
// add your own SplitterPaintingLogic policies you can add upp THAT policy as the
// template parameter
template< class SplitterPaintingLogic >
class WidgetTest1
	: public WidgetFactory< SmartWin::WidgetWindow, WidgetTest1< SplitterPaintingLogic > >
{
	typedef WidgetFactory< SmartWin::WidgetWindow, WidgetTest1< SplitterPaintingLogic > > FactoryType;
public:
	WidgetTest1()
		: m_BorderSize( 4 )
	{}

	typename FactoryType::WidgetChildWindowPtr left;
	typename FactoryType::WidgetChildWindowPtr right;

	typedef WidgetSplitter< SmartWin::WidgetWindow, WidgetTest1, SplitterPaintingLogic > WhichSplitter;

	WhichSplitter * splitter;
	unsigned int m_BorderSize;

	void splitterMoved( WhichSplitter * split, const SmartWin::Point & pt = SmartWin::Point() )
	{
		SmartWin::Rectangle rectSplitter = splitter->getBounds();
		SmartWin::Rectangle rectWindow = FactoryType::getBounds();
		SmartWin::Rectangle leftRc = rectWindow;
		leftRc.size.x = rectSplitter.pos.x - rectWindow.pos.x - m_BorderSize;
		leftRc.size.y = rectWindow.size.y;
		leftRc.pos.x = leftRc.pos.y = 0;
		left->setBounds( leftRc );
		left->updateWidget();

		SmartWin::Rectangle rightRc = rectWindow;
		rightRc.pos.x = rectSplitter.pos.x + splitter->getWidth() - rectWindow.pos.x;
		rightRc.pos.y = 0;

		rightRc.size.x += rightRc.pos.x - m_BorderSize;
		rightRc.size.x = rectWindow.size.x - ( rectSplitter.pos.x + splitter->getWidth() - rectWindow.pos.x );
		rightRc.size.y = rectWindow.size.y;
		right->setBounds( rightRc );
		right->updateWidget();
		if ( split != 0 )
			split->updateWidget();
	}

	void windowSized( const SmartWin::WidgetSizedEventResult & newSize )
	{
		SmartWin::Rectangle rectSplitter = splitter->getBounds();
		SmartWin::Rectangle rectWindow = FactoryType::getBounds();

		rectSplitter.pos.x -= ( rectWindow.pos.x + m_BorderSize );
		rectSplitter.pos.y = 0;
		rectSplitter.size.x = splitter->getWidth();
		rectSplitter.size.y = rectWindow.size.y;
		splitter->setBounds( rectSplitter );

		// Just calling the splitter moved routine to resize the windows...
		splitterMoved( 0 );
	}

	static void btnClickedStatic( WidgetTest1 * window, typename FactoryType::WidgetButtonPtr btn )
	{
		window->createMessageBox().show( btn->getText() );
	}

	void btnClickedMember( typename FactoryType::WidgetButtonPtr btn )
	{
		FactoryType::createMessageBox().show( btn->getText() );
	}

	void movedFunc( const SmartWin::Point & pt )
	{
		MySplitterPainting * split = dynamic_cast< MySplitterPainting * >( splitter );
		if ( split != 0 )
		{
			WhichSplitter * split2 = dynamic_cast< WhichSplitter * >( split );
			split2->setWidth( ( rand()%40 ) + 10 );
			SmartWin::Rectangle rectSplitter = split2->getBounds();
			SmartWin::Rectangle rectWindow = FactoryType::getBounds();

			rectSplitter.pos.x -= (rectWindow.pos.x + 4 /*Border*/);
			rectSplitter.pos.y = 0;
			rectSplitter.size.x = split2->getWidth();
			rectSplitter.size.y = rectWindow.size.y;
			split2->setBounds( rectSplitter );
			split->updateColor();
			splitterMoved( 0 );
		}
	}

	void initAndCreate()
	{
		typename FactoryType::WidgetWindow::Seed windowCS;

		windowCS.background = CreateSolidBrush( RGB( 0, 0, 125 ) );
		windowCS.caption = _T( "WidgetSplitter compile and execute test" );
		windowCS.location.size = SmartWin::Point( 400, 200 );
		FactoryType::createWindow( windowCS );
		FactoryType::onSized( & WidgetTest1::windowSized );
		FactoryType::onMoved( & WidgetTest1::movedFunc );

		typename FactoryType::WidgetChildWindow::Seed childWindowCS;

		childWindowCS.background = CreateSolidBrush( RGB( 0, 0, 125 ) );
		childWindowCS.location = SmartWin::Rectangle( 0, 0, 100, 200 );
		left = FactoryType::createWidgetChildWindow( childWindowCS );
		typename FactoryType::WidgetButton::Seed buttonCS;

		buttonCS.caption = _T( "Testing Left" );
		buttonCS.location = SmartWin::Rectangle( 50, 50, 100, 50 );

		typename FactoryType::WidgetButtonPtr btn1 = left->createButton( buttonCS );
		btn1->onClicked( & WidgetTest1::btnClickedStatic );

		right = FactoryType::createWidgetChildWindow( childWindowCS );
		buttonCS.caption = _T( "Testing Right" );
		typename FactoryType::WidgetButtonPtr btn2 = right->createButton( buttonCS );
		btn2->onClicked( & WidgetTest1::btnClickedMember );

		// Here we are using the template member function to show how to easily
		// change splitter without massive recoding. Since gcc 3.4.2 coffs on the
		// template member function in WidgetFactory which SHOULD have done this
		// with ONE line we must do this explicitly by doing the actual "new"
		// stuff...
		splitter = SmartWin::WidgetCreator< WhichSplitter >::create( this );
		splitter->onMoved( & WidgetTest1::splitterMoved );
		//splitterMoved( 0 );
	}
};

// Unit test for WidgetSplitter, tries to instantiate all different forms of
// WidgetSplitter and also tries to use all functions
int SmartWinMain( Application & app )
{
	// Creating a WidgetTest1 Widget with a booring gray splitter
	WidgetTest1< SmartWin::SplitterThinPaint > * testWnd1 = new WidgetTest1< SmartWin::SplitterThinPaint >;
	testWnd1->initAndCreate();
	testWnd1->setBounds( 100, 100, 450, 200 );

	// Creating another type of window to show how to easily exchange our splitter painting policy/Aspect
	WidgetTest1< SmartWin::SplitterCoolPaint > * testWnd2 = new WidgetTest1< SmartWin::SplitterCoolPaint >;
	testWnd2->initAndCreate();
	testWnd2->setBounds( 150, 150, 450, 200 );

	// Creating another type of window to show how to easily inject your OWN painting policy/Aspect
	WidgetTest1< MySplitterPainting > * testWnd3 = new WidgetTest1< MySplitterPainting >;
	testWnd3->initAndCreate();
	testWnd3->setBounds( 200, 200, 450, 200 );

	// ********************************************************************************************************************************************
	// Follow me now !!!!!!!!!!!!!!!!!
	testWnd3->splitter->setMyCoolColor( RGB( 0, 255, 0 ), RGB( 0, 0, 255 ) );
	testWnd3->setText( _T( "Try to MOVE me!" ) );
	// ********************************************************************************************************************************************

	// Since we have injected our own SplitterPainting Policy here and the
	// WidgetSplitter actually derives from the template argument we can...
	// ...access members and methods of our policy!!!!
	// If we tried to call setMyCoolColor for any of the SmartWin supplied policies
	// we would have gotten compile time errors since (obviously) the setColor
	// member function of the SmartWin supplied Painter Policies DOESN'T EXIST! ***
	// MAGIC *** I LOVE C++!!

	WidgetTest1< MySplitterPainting >::WidgetStaticPtr stat = testWnd3->left->createStatic();
	stat->setBounds( 10, 10, 200, 30 );
	stat->setText( _T( "You just HAVE to try to move this window! ;)" ) );
	return app.run();
}
