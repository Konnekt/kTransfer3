// $Revision: 1.9 $
/*!
  \file Main.cpp
  \brief WidgetGraph sample program
*/
#include "SmartWin.h"
using namespace SmartWin;
#include "WidgetGraph.h"
#include <time.h>

template< typename Numeric >
class WidgetGraphTest
	: public WidgetFactory< WidgetWindow, WidgetGraphTest< Numeric > >
{
	WidgetGraph< Numeric > * itsGraph;

public:
	WidgetGraphTest()
	{}

	void simpleXYData()
	{
		this->setBounds( 0, 0, 250, 200 );
		this->setText( _T( "WidgetGraph test of X - Y data graph" ) );
		short xdata[] =
		{ 4, 6, 8, 10, 12
		};
		itsGraph->setXData( _T( "length" ), xdata, & xdata[sizeof( xdata ) / sizeof( short )] );

		short ydata[] =
		{ 36, 56, 84, 120, 164
		};
		itsGraph->setYData( _T( "weight" ), ydata, & ydata[sizeof( ydata ) / sizeof( short )] );
	}

	void histogram()
	{
		this->setBounds( 0, 400, 250, 200 );
		this->setText( _T( "Histogram" ) );
		const int N = 5000;
		int xdata[N];
		srand( time( NULL ) );
		for ( int i = 0; i < N; i++ )
		{
			xdata[i] = ( rand() & 0xfff ) - ( rand() & 0xfff );
		}
		itsGraph->setXData( _T( "rand - rand" ), xdata, & xdata[N] );
		itsGraph->setText( _T( "Differences between uniform numbers" ) );
	}

	void socialSecurity()
	{
		this->setBounds( 0, 200, 500, 200 );

		// Assets at end of year
		itsGraph->setText( _T( "U.S. Social Security Trust fund value in millions from 1937 to 2004" ) );

		int xdata[] =
		{ 1937, 1938, 1939,
						1940, 1941, 1942, 1943, 1944, 1945, 1946, 1947, 1948, 1949,
						1950, 1951, 1952, 1953, 1954, 1955, 1956, 1957, 1958, 1959,
						1960, 1961, 1962, 1963, 1964, 1965, 1966, 1967, 1968, 1969,
						1970, 1971, 1972, 1973, 1974, 1975, 1976, 1977, 1978, 1979,
						1980, 1981, 1982, 1983, 1984, 1985, 1986, 1987, 1988, 1989,
						1990, 1991, 1992, 1993, 1994, 1995, 1996, 1997, 1998, 1999,
						2000, 2001, 2002, 2003, 2004
						};

		itsGraph->setXData( _T( "year" ), xdata, & xdata[sizeof( xdata ) / sizeof( int )] );

		int ydata[] =
		{ 766, 1132, 1724,
						  2031, 2762, 3688, 4820, 6005, 7121, 8150, 9360, 10722, 11816,
						 13721, 15540, 17442, 18707, 20576, 21663, 22519, 22393, 21864, 20141,
						 20324, 19725, 18337, 18480, 19125, 18235, 20570, 24222, 25704, 30082,
						 32454, 33789, 35318, 36487, 37777, 36987, 35388, 32491, 27520, 24660,
						 22823, 21490, 22088, 19672, 27117, 35842, 39081, 62149, 102899, 155063,
						214197, 267849, 319150, 369322, 413460, 458502, 514026, 589121, 681645, 798812,
						930986, 1071540, 1217497, 1355330, 1500622
						};

		itsGraph->setYData( _T( "value" ), ydata, & ydata[sizeof( xdata ) / sizeof( int )] );
	}

	void doubleXYData()
	{
		this->setBounds( 250, 0, 250, 200 );
		this->setText( _T( "WidgetGraph test of double X - Y data graph" ) );
		double xdata[] =
		{
			- 2.4, 5.3, 2.3, - 3.5, 4.2, 6.4, 4.3, 8.2
		};
		itsGraph->setXData( _T( "length" ), xdata, & xdata[sizeof( xdata ) / sizeof( double )] );

		double x2data[] =
		{ 23.4, 43.2, 55.3, 32.4, 32.4, 23.8, 22.4, 19.2
		};
		itsGraph->setXData( _T( "Width" ), x2data, & x2data[sizeof( x2data ) / sizeof( double )] );

		double ydata[] =
		{ 0.34, 0.56, 0.84, 2.4, 5.3, 6.43, 11.32, 12.34
		};
		itsGraph->setYData( _T( "weight" ), ydata, & ydata[sizeof( ydata ) / sizeof( double )] );
	}

	void initAndCreate()
	{
		//typename WidgetFactory< sw::WidgetWindow, WidgetGraphTest< Numeric > >::Seed windowCS;
		WidgetWindow::Seed windowCS;

		windowCS.location = SmartWin::Rectangle( 0, 0, 100, 100 );
		windowCS.background = CreateSolidBrush( RGB( 0, 0, 125 ) );
		windowCS.caption = _T( "Test" );
		this->createWindow( windowCS );

		// Creating custom Widget
		itsGraph = new WidgetGraph< Numeric >( this );
		itsGraph->initAndCreate();

		Numeric mytype;
		if ( typeid( short ) == typeid( mytype ) ) simpleXYData();
		if ( typeid( long ) == typeid( mytype ) ) socialSecurity();
		if ( typeid( double ) == typeid( mytype ) ) doubleXYData();
		if ( typeid( int ) == typeid( mytype ) ) histogram();

		SmartUtil::tstring title( _T( "WidgetGraph sample for type " ) );
		title += typeid( mytype ).name();
		this->setText( title );

		layout();
		onSized( & WidgetGraphTest::isResized );
	}

	void isResized( const WidgetSizedEventResult & sz )
	{
		layout();
	}

	void layout()
	{
		SmartWin::Rectangle r( this->getClientAreaSize() );
		// r = r.shrink( 6, 6 );
		itsGraph->setBounds( r );
	}
};

// Unit test for WidgetGraph
int SmartWinMain( Application & app )
{
	WidgetGraphTest< short > * testWndShort = new WidgetGraphTest< short >;
	testWndShort->initAndCreate();

	WidgetGraphTest< double > * testWndDouble = new WidgetGraphTest< double >;
	testWndDouble->initAndCreate();

	WidgetGraphTest< int > * testWndInt = new WidgetGraphTest< int >;
	testWndInt->initAndCreate();

	WidgetGraphTest< long > * testWndLong = new WidgetGraphTest< long >;
	testWndLong->initAndCreate();

	return app.run();
}
