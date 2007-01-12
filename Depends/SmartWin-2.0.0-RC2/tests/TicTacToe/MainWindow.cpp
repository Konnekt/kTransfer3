// $Revision: 1.9 $
#include "MainWindow.h"
#include "resource.h"

void MainWindow::painting( SmartWin::Canvas & canvas )
{
	SmartWin::Pen pen( canvas, color );
	int idx;
	for ( idx = 0; idx < 4; ++idx )
	{
		canvas.line( idx * 100 + 10, 10, idx * 100 + 10, 310 );
		canvas.line( 10, idx * 100 + 10, 310, idx * 100 + 10 );
	}
	assert( hexa.size() == 9 );
	for ( idx = 0; idx < 9; ++idx )
	{
		if ( hexa[idx] != NONE )
		{
			unsigned xCorner = ( idx % 3 ) * 100 + 10;
			unsigned yCorner = ( idx / 3 ) * 100 + 10;
			if ( HUMAN == hexa[idx] )
			{
				canvas.line( xCorner, yCorner, xCorner + 100, yCorner + 100 );
				canvas.line( xCorner + 100, yCorner, xCorner, yCorner + 100 );
			}
			else if ( COMPUTER == hexa[idx] )
			{
				canvas.ellipse( xCorner, yCorner, xCorner + 100, yCorner + 100 );
			}
		}
	}
}

bool MainWindow::keyPressed( int key )
{
	int x = key - '0';
	if ( x < 0 || x > 8 )
		return false;
	else
	{
		hexa[x] = HUMAN; // Note! Doesn't even check if grid cell is occupied...
		doComputerMove();
		updateWidget();
		return true;
	}
	return true;
}

void MainWindow::winner( HexaVal val )
{
	updateWidget();
	createMessageBox().show( SmartUtil::tstring( _T( "And the winner is: " ) ) + ( val == COMPUTER ? _T( "MEEEE!!!" ) : _T( "You're much to intelligent for me..." ) ) );
	for ( std::vector< HexaVal >::iterator idx = hexa.begin();
		idx != hexa.end();
		++idx )
		* idx = NONE;
}

bool MainWindow::checkWinner()
{
	if ( ( hexa[0] & hexa[1] ) == hexa[2] && hexa[2] != NONE )
	{
		winner( hexa[0] );
		return true;
	}
	if ( ( hexa[3] & hexa[4] ) == hexa[5] && hexa[5] != NONE )
	{
		winner( hexa[3] );
		return true;
	}
	if ( ( hexa[6] & hexa[7] ) == hexa[8] && hexa[8] != NONE )
	{
		winner( hexa[6] );
		return true;
	}
	if ( ( hexa[0] & hexa[3] ) == hexa[6] && hexa[6] != NONE )
	{
		winner( hexa[0] );
		return true;
	}
	if ( ( hexa[1] & hexa[4] ) == hexa[7] && hexa[7] != NONE )
	{
		winner( hexa[1] );
		return true;
	}
	if ( ( hexa[2] & hexa[5] ) == hexa[8] && hexa[8] != NONE )
	{
		winner( hexa[2] );
		return true;
	}
	if ( ( hexa[0] & hexa[4] ) == hexa[8] && hexa[8] != NONE )
	{
		winner( hexa[0] );
		return true;
	}
	if ( ( hexa[2] & hexa[4] ) == hexa[6] && hexa[6] != NONE )
	{
		winner( hexa[2] );
		return true;
	}
	return false;
}

void MainWindow::init()
{
	// Can't call this one in Constructor since object isn't finished constructed
	onActivate( & MainWindow::activate );
}

void MainWindow::doComputerMove()
{
	if ( checkWinner() )
	{
		return;
	}
	bool full = true;
	for ( std::vector< HexaVal >::iterator idx = hexa.begin();
		idx != hexa.end();
		++idx )
	{
		if ( NONE == * idx )
			full = false;
	}
	if ( full )
	{
		updateWidget();
		createMessageBox().show( _T( "Finished, nobody did win, starting over again..." ) );
		for ( std::vector< HexaVal >::iterator idx = hexa.begin();
			idx != hexa.end();
			++idx )
			* idx = NONE;
	}
	else
	{
		while ( true )
		{
			// Not very intelligent routine... ;)
			int move = rand() % 8;
			if ( NONE == hexa[move] )
			{
				hexa[move] = COMPUTER;
				break;
			}
		}
	}
	checkWinner();
}

void MainWindow::clicked( const SmartWin::MouseEventResult & mouse )
{
	setActive();
	int x = ( mouse.pos.x - 10 ) / 100;
	int y = ( mouse.pos.y - 10 ) / 100;
	if ( mouse.pos.x < 10 || x > 2 || mouse.pos.y < 10 || y > 2 || hexa[x + ( y * 3 )] != NONE )
		return;
	hexa[x + ( y * 3 )] = HUMAN; // Note! Doesn't even check if grid cell is occupied...
	doComputerMove();
	updateWidget();
}

void MainWindow::clicked2( const SmartWin::MouseEventResult & mouse )
{
	maximize();
}

void MainWindow::clicked3( const SmartWin::MouseEventResult & mouse )
{
	color = RGB( rand()%255, rand()%255, rand()%255 );
}

void MainWindow::onClickedForFileChange( const SmartWin::MouseEventResult & mouse )
{
	if ( itsHandleFileChange == INVALID_HANDLE_VALUE )
	{
		createMessageBox().show( _T( "Waiting now..." ) );

		itsHandleFileChange = ::FindFirstChangeNotification(
			SmartWin::Application::instance().getModulePath().c_str(),
			FALSE, //bSubTree
			FILE_NOTIFY_CHANGE_FILE_NAME );

		// specify the event handler
		itsSignalConnection =
			itsPtrSignalFileChange->connect( boost::bind( & MainWindow::onFileChanged, this ) );
		// register the wait event in the application
		SmartWin::Application::instance().addWaitEvent( itsHandleFileChange, itsPtrSignalFileChange );
	}
}

void MainWindow::onFileChanged()
{
	SmartWin::Application::instance().removeWaitEvent( itsHandleFileChange );

	::FindCloseChangeNotification( itsHandleFileChange );

	itsSignalConnection.disconnect();
	//itsPtrSignalFileChange->disconnect(0);

	createMessageBox().show( _T( "A file has changed!" ) );

	itsHandleFileChange = INVALID_HANDLE_VALUE;
}

void MainWindow::activate( bool active )
{
	isActive = active;
	setText( isActive ? _T( "ACTIVE" ) : _T( "Not active..." ) );
}

MainWindow::MainWindow()
	: isActive( true ),
	itsHandleFileChange( INVALID_HANDLE_VALUE ),
	itsPtrSignalFileChange( new Signal )
{
	WidgetWindow::Seed windowCS;

	windowCS.caption = _T( "Tic Tac SmartWin" );
	windowCS.style = WS_VISIBLE | WS_OVERLAPPEDWINDOW;
	windowCS.exStyle = 0;
	windowCS.location = SmartWin::Rectangle( 400, 400, 330, 345 );
	createWindow( windowCS ); // Creation though customization of the creational structure.

	setMaximizeBox( false );
	onPainting( & MainWindow::painting );
	onLeftMouseUp( & MainWindow::clicked );
	//onRightMouseUp( &MainWindow::clicked2 );
	onLeftMouseDown( & MainWindow::clicked3 );
	onRightMouseDown( & MainWindow::onClickedForFileChange );
	setIconSmall( IDI_ICON1 );
	setIconLarge( IDI_ICON1 );
	setCursor( IDC_CURSOR1 );
	onKeyPressed( & MainWindow::keyPressed );

	// Emptying the hexagon vector
	hexa.push_back( NONE );
	hexa.push_back( NONE );
	hexa.push_back( NONE );
	hexa.push_back( NONE );
	hexa.push_back( NONE );
	hexa.push_back( NONE );
	hexa.push_back( NONE );
	hexa.push_back( NONE );
	hexa.push_back( NONE );
}

MainWindow::~MainWindow()
{
	if ( itsHandleFileChange != INVALID_HANDLE_VALUE )
	{
		::FindCloseChangeNotification( itsHandleFileChange );
	}
}
