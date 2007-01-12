// $Revision: 1.7 $

#include "SmartWin.h"
#include <vector>
#include <assert.h>
using namespace SmartWin;

#include "io/InDialog.h"

// This class represents a "generic graph Widget" which can be reused in several projects.

template< typename Numeric >
class WidgetGraph :
	public WidgetFactory< WidgetWindow, WidgetGraph< Numeric > >
{
public:
	explicit WidgetGraph( SmartWin::Widget * parent )
		: Widget( parent ),
		itsPenWidth( 0 ),
		itsHistoSections( 5 ),
		itsAxisColor( RGB( 180, 70, 0 ) ),
		itsPointColor( RGB( 100, 80, 0 ) ),
		itsXdex( 0 ),
		itsYdex( 0 )
	{
		axisPresent = true;
		::SetParent( WidgetGraph< Numeric >::handle(), WidgetGraph< Numeric >::getParent()->handle() );
	}

	void initAndCreate()
	{
		WidgetWindow::Seed windowCS;

		windowCS.exStyle = WS_EX_TOOLWINDOW;
		windowCS.style = WS_CHILD | WS_THICKFRAME | WS_CAPTION | WS_VISIBLE;
		this->createWindow( windowCS );

		onSized( & WidgetGraph::isResized );
		onPainting( & WidgetGraph::painting );
		onRightMouseDown( & WidgetGraph::chooseProperties );
	}

	template< class NumIter >
	void setXData( const SmartUtil::tstring & inXAxisTitle, NumIter beg, NumIter end )
	{
		setData( itsX, inXAxisTitle, beg, end );
	}

	template< class NumIter >
	void setYData( const SmartUtil::tstring & inYAxisTitle, NumIter beg, NumIter end )
	{
		setData( itsY, inYAxisTitle, beg, end );
//      itsY.setData( beg, end );
//      calcStrSizes( itsY );
	}

private:

	class NumericExtremed
	{
	public:
		SmartUtil::tstring itsTitle;
		std::vector< Numeric > itsData;
		Numeric itsMin, itsMax;
		SmartUtil::tstring itsMinStr, itsMaxStr;
		Point itsMinSize, itsMaxSize;

		bool allIncreasing;

		template< class NumIter >
		void setData( NumIter beg, NumIter end )
		{
			itsData.assign( beg, end );
			setMaxMin();
		}

		void setMaxMin()
		{
			size_t sz = itsData.size();
			if ( 0 == sz ) return;

			allIncreasing = true;
			itsMin = itsMax = itsData[0];

			for ( size_t i = 1; i < sz; ++i )
			{
				if ( itsData[i] < itsMin )
				{
					itsMin = itsData[i];
					allIncreasing = false;
				}

				if ( itsData[i] > itsMax )
					itsMax = itsData[i];
			}

			if ( allIncreasing )
			{
				for ( size_t i = 1; i < sz; ++i )
				{
					if ( itsData[i] < itsData[i - 1] )
					{
						allIncreasing = false;
						break;
					}
				}
			}
		}
	};

	vector< NumericExtremed > itsX, itsY;
	int itsXdex, itsYdex;

	COLORREF itsAxisColor, itsPointColor;
	bool axisPresent;
	int itsPenWidth;
	int itsHistoSections;

	template< class NumIter >
	void setData( vector< NumericExtremed > & vdata, const SmartUtil::tstring & inTitle,
				  NumIter beg, NumIter end )
	{
		vdata.push_back( NumericExtremed() );

		NumericExtremed & dat = vdata[ vdata.size() - 1 ];

		dat.setData( beg, end );
		dat.itsTitle = inTitle;
		calcStrSizes( dat );
	}

	void labelXYAxis( Canvas & canvas, SmartWin::Rectangle & rc,
					SmartWin::Rectangle & graphRect )
	{
		NumericExtremed & xDat = itsX[ itsXdex ];
		NumericExtremed & yDat = itsY[ itsYdex ];

		labelAxis( canvas, xDat, yDat, rc, graphRect );
	}

	void labelAxis( Canvas & canvas,
					NumericExtremed & xDat, NumericExtremed & yDat,
					SmartWin::Rectangle & rc,
					SmartWin::Rectangle & graphRect )
	{
		int padX = 4, padY = 4;
		Pen pen( canvas, itsAxisColor );
	//  canvas.rectangle( 0,0, rc.size.x, rc.size.y );

		Point WideYLabel = yDat.itsMaxSize;
		WideYLabel.maxOf( xDat.itsMinSize ); // WideYLabel.x has the widest y label width
		Point yLabelSize = getTextSize( yDat.itsTitle );
		WideYLabel.maxOf( yLabelSize ); // Allocate space for y label
		WideYLabel.x += padX;

		Point HighXLabel = xDat.itsMaxSize;
		HighXLabel.maxOf( xDat.itsMinSize ); // HighXLabel.y has tallest X label height
		HighXLabel.y += padY;

		graphRect = graphRect.upperLeftAdjust( Point( WideYLabel.x, yDat.itsMaxSize.y >> 1 ) );
		graphRect = graphRect.lowerRightAdjust( Point( - ( xDat.itsMaxSize.x >> 1 ), - HighXLabel.y ) );

		// Draw X and Y axis
		canvas.line( graphRect.pos.x, graphRect.pos.y,
					graphRect.pos.x, graphRect.pos.y + graphRect.size.y );
		canvas.lineTo( graphRect.pos.x + graphRect.size.x, graphRect.pos.y + graphRect.size.y );

		// Label min X axis
		canvas.extTextOut( xDat.itsMinStr,
							graphRect.pos.x - ( xDat.itsMinSize.x >> 1 ),
							graphRect.pos.y + graphRect.size.y + padY );

		// X axis label
		Point xLabelSize = getTextSize( xDat.itsTitle );
		canvas.extTextOut( xDat.itsTitle,
							graphRect.pos.x + ( ( graphRect.size.x - xLabelSize.x ) >> 1 ),
							graphRect.pos.y + graphRect.size.y + padY );

		// Label max X axis
		canvas.extTextOut( xDat.itsMaxStr,
							graphRect.pos.x + graphRect.size.x - ( xDat.itsMaxSize.x >> 1 ),
							graphRect.pos.y + graphRect.size.y + padY );

		// Label min Y axis
		canvas.extTextOut( yDat.itsMinStr,
							graphRect.pos.x - ( yDat.itsMinSize.x + padX ),
							graphRect.pos.y + graphRect.size.y - ( yDat.itsMinSize.y >> 1 ) );

		canvas.extTextOut( yDat.itsTitle,
							graphRect.pos.x - ( yLabelSize.x + padX ),
							graphRect.pos.y + ( graphRect.size.y >> 1 ) );

		// Label max Y axis
		canvas.extTextOut( yDat.itsMaxStr,
							graphRect.pos.x - ( yDat.itsMaxSize.x + padX ),
							graphRect.pos.y - ( yDat.itsMaxSize.y >> 1 ) );
	}

	int xMap( int dex, SmartWin::Rectangle & rc )
	{
		NumericExtremed & xDat = itsX[ itsXdex ];
		return xMap( dex, xDat, rc );
	}

	int xMap( int dex, NumericExtremed & xDat, SmartWin::Rectangle & rc )
	{
		float factor = ( float ) rc.size.x / ( xDat.itsMax - xDat.itsMin );
		int x = ( int ) ( ( xDat.itsData[dex] - xDat.itsMin ) * factor );
		return x + rc.pos.x;
	}

	int yMap( int dex, SmartWin::Rectangle & rc )
	{
		NumericExtremed & yDat = itsY[ itsYdex ];
		return yMap( dex, yDat, rc );
	}

	int yMap( int dex, NumericExtremed & yDat, SmartWin::Rectangle & rc )
	{
		float factor = ( float ) rc.size.y / ( yDat.itsMax - yDat.itsMin );
		int y = ( int ) ( ( yDat.itsData[dex] - yDat.itsMin ) * factor );
		y = rc.size.y - ( y + 1 );
		return y + rc.pos.y;
	}

	void drawGraph( Canvas & canvas, SmartWin::Rectangle & rc )
	{
		NumericExtremed & xDat = itsX[ itsXdex ];

		size_t sz = xDat.itsData.size();
		if ( 0 == sz ) return;

		if ( xDat.allIncreasing )
		{
			// Draw lines between each point
			Pen pen( canvas, itsPointColor, itsPenWidth );
			canvas.moveTo( xMap( 0, rc ), yMap( 0, rc ) );

			for ( int dex = 1; dex < ( int ) sz; ++dex )
			{
				canvas.lineTo( xMap( dex, rc ), yMap( dex, rc ) );
			}
		}
		else
		{
			// Draw little squares at each X-Y point.
			SmartWin::Brush brush( canvas, itsPointColor );

			int x, y, dim = 4;
			for ( int dex = 0; dex < ( int ) sz; ++dex )
			{
				x = xMap( dex, rc );
				y = yMap( dex, rc );
				canvas.rectangle( x - dim, y - dim, x + dim, y + dim );
			}
		}
	}

	void drawHistogram( Canvas & canvas, SmartWin::Rectangle & rc, SmartWin::Rectangle & graphRect )
	{
		long border = 8;
//      rc.shrink( border );

		NumericExtremed & xDat = itsX[ itsXdex ];

		size_t sz = xDat.itsData.size();
		if ( 0 == sz ) return;

		Numeric range = xDat.itsMax - xDat.itsMin;
		double blockRange = ( double ) range / itsHistoSections;
		vector< Numeric > counts, blocknum;
		for ( int i = 0; i < itsHistoSections; i++ )
		{
			counts.push_back( 0 );
			blocknum.push_back( static_cast< Numeric >( ( i * blockRange ) + xDat.itsMin ) );
		}
		blocknum.push_back( xDat.itsMax );

		for ( size_t i = 0; i < sz; i++ )
		{
			int block = static_cast< int >( ( xDat.itsData[i] - xDat.itsMin ) / blockRange );
			if ( block < itsHistoSections )
				counts[ block ]++;
			else
				counts[ block - 1 ]++;
		}
		// Now  counts[0..blocks-1] has the number of occurences in the range.

		int maxCount = 0;
		for ( int i = 0; i < itsHistoSections; i++ )
		{
			if ( static_cast< int >( counts[i] ) > maxCount ) maxCount = static_cast< int >( counts[i] );
		}
		// Now we know maxCount, the most occurences in a range.
		NumericExtremed xBlock, yCount;
		xBlock.itsTitle = xDat.itsTitle;
		xBlock.setData( blocknum.begin(), blocknum.end() );
		calcStrSizes( xBlock );

		yCount.itsTitle = _T( "Counts" );
		yCount.setData( counts.begin(), counts.end() );
		yCount.itsMin = 0;
		calcStrSizes( yCount );

		labelAxis( canvas, xBlock, yCount, rc, graphRect );

		Pen pen( canvas, itsPointColor, itsPenWidth );
		int yzero = graphRect.pos.y + graphRect.size.y - 1;
		canvas.moveTo( graphRect.pos.x, yzero );

		for ( int dex = 0; dex < itsHistoSections; ++dex )
		{
			int x = xMap( dex, xBlock, graphRect );
			int y = yMap( dex, yCount, graphRect );
			canvas.lineTo( x, y );
			canvas.lineTo( xMap( dex + 1, xBlock, graphRect ), y );
		}
		canvas.lineTo( xMap( itsHistoSections, xBlock, graphRect ), yzero );
	}

	void isResized( const WidgetSizedEventResult & sz )
	{
		this->updateWidget();
	}

	void painting( Canvas & canvas )
	{
		// Control is "transparent"
		canvas.setBkMode( true );

		// Getting client area size
		SmartWin::Rectangle rc( this->getClientAreaSize() );
		SmartWin::Rectangle graphRect( rc );

		if ( 0 == itsY.size() )
		{
			// Draw histogram of X only data.
			drawHistogram( canvas, rc, graphRect );
		}
		else
		{
			// Draw X-Y data.
			if ( axisPresent )
			{
				labelXYAxis( canvas, rc, graphRect );
			}

			Pen graphPen( canvas, RGB( 0, 0, 150 ) );
			drawGraph( canvas, graphRect );
		}
	}

	Point getNumericSize( Numeric n, SmartUtil::tstring & str )
	{
		std::stringstream ss;
		ss << n;
		str = ss.str();
		return this->getTextSize( str );
	}

	void calcStrSizes( NumericExtremed & xy )
	{
		xy.itsMinSize = getNumericSize( xy.itsMin, xy.itsMinStr );
		xy.itsMaxSize = getNumericSize( xy.itsMax, xy.itsMaxStr );
	}

	void chooseProperties( const SmartWin::MouseEventResult & mouse )
	{
		InDialog myInDialog( this, "Graph parameters" );
		myInDialog
			.add( _T( "Axis present?" ), & axisPresent )
			.add( _T( "Point color ..." ), & itsPointColor )
			.add( _T( "Axis color ..." ), & itsAxisColor )
			.add( _T( "Graph width" ), & itsPenWidth );

		if ( 0 == itsY.size() )
		{
			myInDialog.add( _T( "Histogram sections" ), & itsHistoSections );
		}

		if ( itsX.size() > 1 )
		{
			vector< SmartUtil::tstring > PossibleStrings;
			 for ( int i = 0; i < ( int ) itsX.size(); i++ )
			 {
				PossibleStrings.push_back( itsX[i].itsTitle );
			 }
			ChoiceString xChoice( PossibleStrings, this->itsXdex );
			myInDialog
			.add( _T( "X Axis variable" ), & xChoice );

			if ( IDCANCEL == myInDialog.createDialog() ) return;
			this->itsXdex = xChoice.itsChoosenIndex;
		}
		else
		{
			myInDialog.add( _T( "X axis title:" ), & itsX[ itsXdex ].itsTitle );

			if ( 0 != itsY.size() )
			{
				myInDialog.add( _T( "Y axis title:" ), & itsY[ itsYdex ].itsTitle );
			}

			if ( IDCANCEL == myInDialog.createDialog() ) return;
		}

		this->updateWidget();
	}

protected:
	// When creating Widgets it's good practise to make DTOR protected to disable
	// "stack creation" of them since they destroy themself in the WM_DESTROY
	// message
	virtual ~WidgetGraph()
	{}
};
