// $Revision: 1.9 $
#include "SmartWin.h"
#include <vector>
#include <math.h>

class MainWindow
	: public sw::WidgetFactory< sw::WidgetWindow, MainWindow >,
	public sw::HeartBeat
{
	typedef boost::signal < void () > Signal;
	typedef boost::shared_ptr< Signal > SignalPtr;

	bool isActive;
private:
	COLORREF color;
	COLORREF bgColor;
	typedef enum HexaVal
	{
		NONE, COMPUTER, HUMAN
	};
	std::vector< HexaVal > hexa;
	void painting( SmartWin::Canvas & canvas );
	void winner( HexaVal val );
	bool checkWinner();
	void doComputerMove();
	bool keyPressed( int key );
	void clicked( const SmartWin::MouseEventResult & mouse );
	void clicked2( const SmartWin::MouseEventResult & mouse );
	void clicked3( const SmartWin::MouseEventResult & mouse );
	void onClickedForFileChange( const SmartWin::MouseEventResult & mouse );
	void activate( bool isActive );

	// in order to test the synchronization objects
	HANDLE itsHandleFileChange;
	SignalPtr itsPtrSignalFileChange;
	boost::signals::connection itsSignalConnection;
	void onFileChanged();

public:
	void init();
	virtual void tick()
	{
		if ( !isActive )
			return;

		static DWORD lastTicks = ::GetTickCount();
		DWORD thisTicks = ::GetTickCount();
		if ( thisTicks - 50 > lastTicks )
		{
			static int x = - 100;
			static int y = - 100;
			static sw::Rectangle rectThis( getClientAreaSize() );
			static unsigned iteration = 0;
			iteration += 1;
			SmartWin::UpdateCanvas canvas( this );
			SmartWin::Brush brush( canvas, bgColor );
			sw::HdcModeSetter tmp( canvas, R2_NOT );

			// Removing old circle
			canvas.ellipse( x, y, x + 30, y + 30 );
			x = ( iteration%( rectThis.size.x + 30 ) ) - 30;
			y = ( int )( cos( ( ( double ) iteration ) / 100 ) * 100 ) + 100;

			// Making new circle
			bgColor = RGB( rand()%255, rand()%255, rand()%255 );
			canvas.ellipse( x, y, x + 30, y + 30 );
			lastTicks = thisTicks;

			// Setting text...
			setText( boost::lexical_cast< SmartUtil::tstring >( iteration ) );
		}
	}
	MainWindow();
	~MainWindow();
};
