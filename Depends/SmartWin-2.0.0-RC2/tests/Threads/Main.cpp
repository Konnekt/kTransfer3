// $Revision: 1.7 $
// Threads SmartWin sample application project
// Demonstrates how to use the thread classes in SmartWin...
// This example shows how to do thread safe GUI development in SmartWin but I have
// to warn you that in this example there are several threads accessing and
// manipulating the GUI parts which is normally an Anti Design Pattern because of
// the complexity involved, normally you would have one or more "worker threads"
// which do time demanding jobs like for instance making backups, saving very large
// files to disc etc, but only one thread actually manipulating the GUI layer... So
// this is probably NOT a good example to blindfolded copy and implement in your
// own application!

#include "SmartWin.h"

using namespace SmartWin;

// Our Main Widget
class WidgetTest1
	: public WidgetFactory< WidgetWindow, WidgetTest1 >
{
	// Adding a couple of text Widgets
	WidgetTextBoxPtr text1;
	WidgetTextBoxPtr text2;

	// Adding our pause thread buttons
	WidgetButtonPtr button1;
	WidgetButtonPtr button2;

	// Adding a couple of threads
	Utilities::Thread itsThread1;
	Utilities::Thread itsThread2;
	Utilities::Thread itsThread3;
	Utilities::Thread itsExitThread;

	// This is a shared object which several threads needs read/write access
	// therefore we need to fiddle with the ThreadLock and CriticalSetion classes
	// later...
	SmartUtil::tstring sharedObject;

public:

	// This one becomes true to Signal to the threads that the application is
	// trying to exit... When this one becomes true the threads cleans up their
	// business and returns naturally! We could have used the Thread::terminate
	// function, but it's dirty and won't let the threads do house cleaning before
	// they exit, therefore we use this approach instead.. Note it's public to make
	// it accessible from neThread3 which is a static function
	bool isClosing;

	// Constructor only initializing isClosing to true, no other purpose...
	WidgetTest1()
		: isClosing( false )
	{}

	// Checks if the text of the text box is more then a threshold
	void checkIfTooLong( WidgetTextBoxPtr text )
	{
		if ( text->getText().size() > 500 )
			text->setText( _T( "" ) );
	}

	// Thread 1 function
	unsigned long newThread1( SmartUtil::tstring & message )
	{
		std::stringstream str;
		int idx = 0;

		// Loop until closing is true
		while ( !isClosing )
		{
			// Aquire lock
			Utilities::ThreadLock lock( getCriticalSection() );

			// Checking if text of textbox is too long and needs resetting
			checkIfTooLong( text1 );

			// Manipulating shared resource
			TCHAR tmp = * sharedObject.begin();
			sharedObject = SmartUtil::tstring( sharedObject.begin() + 1, sharedObject.end() );
			sharedObject.push_back( tmp );

			// Doing time demanding locked operation
			str.str( _T( "" ) );
			str.clear();
			str << ++idx;
			text1->setText( text1->getText() + ( message + _T( " : " ) + str.str() ) + "    " + sharedObject + _T( "\r\n" ) );
			text1->showCaret();

			// Sleeping for 40 milliseconds
			::Sleep( 40 );

			// Note!
		// Here our lock will automatically be freed since the ThreadLock object
		// will be destroyed...
		}
		return 0;
	}

	// thread 2 function
	unsigned long newThread2( SmartUtil::tstring & message )
	{
		int idx = 0;

		// Loop until closing
		while ( !isClosing )
		{
			// Aquiring lock in a different way here (not so safe since we don't have RAII here)
			getCriticalSection().lock();

			// Checking if text of textbox is too long and needs resetting
			checkIfTooLong( text2 );

			// Manipulating shared resource
			TCHAR tmp = * ( sharedObject.end() - 1 );
			sharedObject = SmartUtil::tstring( sharedObject.begin(), sharedObject.end() - 1 );
			sharedObject.push_back( tmp );

			// Doing time demanding locked operation
			std::stringstream str;
			str << ++idx;
			text2->setText( text2->getText() + ( message + _T( " : " ) + str.str() + "    " + sharedObject + _T( "\r\n" ) ) );
			text2->showCaret();

			// Sleeping for 50 milliseconds
			::Sleep( 50 );

			// Note here that if we got an exception between the lock above and
			// this line then the critical section will never be released and the
			// application will STALL, therefore the prefered way of aquiring a
			// critical section is the one demonstrated in newThread1 function!!
			getCriticalSection().unlock();
		}
		return 0;
	}

	// thread 3 function, note this one is a static/global event handler therefore
	// it takes a pointer to the Widget initiating the thread
	static unsigned long newThread3( WidgetTest1 * widget, int & x )
	{
		// Loop until closing
		while ( !widget->isClosing )
		{
			widget->setText( _T( " Howdy" ) );

			// Sleeping for 500 milliseconds
			::Sleep( 200 );

			widget->setText( _T( " Dudy Woody!" ) );

			// Sleeping for 500 milliseconds
			::Sleep( 200 );
		}
		return 0;
	}

	unsigned long forkPause( boost::tuple < Utilities::Thread, WidgetButton * > & param )
	{
		// Aquiring lock, we must get a lock here otherwise we may risc that
		// another thread which owns the lock gets suspended and the lock will be
		// locked by a suspended thread...
		Utilities::ThreadLock lock( getCriticalSection() );
		if ( ( param.get< 1 >() )->getText().find( _T( "Pause" ) ) != SmartUtil::tstring::npos )
		{
			param.get< 0 >().suspend();
			( param.get< 1 >() )->setText( _T( "Resume thread (Start thread)" ) );
		}
		else
		{
			param.get< 0 >().resume();
			( param.get< 1 >() )->setText( _T( "Suspend thread (Pause thread)" ) );
		}
		return 0;
	}

	void pauseClicked1( WidgetButtonPtr clickedButton )
	{
		// We must do this in another thread for safety reasons since this one is
		// called on the main thread and that other thread might be dependent on
		// sending a message to a Widget (which obviously WON'T be handled since
		// this thread would be halted while waiting for getting a lock)
		Utilities::Thread pauseThread = fork( boost::tuple < Utilities::Thread, WidgetButton * >( itsThread1, clickedButton ), & WidgetTest1::forkPause );
	}

	void pauseClicked2( WidgetButtonPtr clickedButton )
	{
		// We must do this in another thread for safety reasons since this one is
		// called on the main thread and that other thread might be dependent on
		// sending a message to a Widget (which obviously WON'T be handled since
		// this thread would be halted while waiting for getting a lock)
		Utilities::Thread pauseThread = fork( boost::tuple < Utilities::Thread, WidgetButton * >( itsThread2, clickedButton ), & WidgetTest1::forkPause );
	}

	bool closeIndeed()
	{
		// Letting the exitApplication thread get to return and free it's resources
		// before closing the application
		Utilities::Thread::waitForObject( itsExitThread );
		return true;
	}

	unsigned long exitApplication( int & x /* Dummy variable*/ )
	{
		// "Signaling"
		isClosing = true;

		// Starting threads again (in case they are suspended) since we NEED them
		// to run in order to be able to return from them
		itsThread1.resume();
		itsThread2.resume();

		// Waiting for threads to end
		std::vector< Utilities::Thread > vecThreads;
		vecThreads.push_back( itsThread1 );
		vecThreads.push_back( itsThread2 );
		vecThreads.push_back( itsThread3 );
		Utilities::Thread::waitForObjects( vecThreads );

		// Setting close to "guaranteed closing"
		onClosing( & WidgetTest1::closeIndeed );

		// Posting close event (asyncron since we need to return from this function
		// BEFORE handling the close event)
		close( true );
		return 0;
	}

	bool closeApp()
	{
		// Note!
		// Here we delegate the actually exiting of the application to a THIRD
		// thread since we're depending on waiting for our thread1 and thread2 to
		// exit before we can actually close the application, and the signaling of
		// closing those threads would require a Thread::waitForObjects call and
		// that would LOCK this thread (the main thread) and those threads could be
		// dependant on syncronized messages from the main thread to finish which
		// obviously would NOT be handled since the main thread would be stuck
		// waiting for those threads to finish... I never said multi threading
		// would be _EASY_ in SmartWin... Multi Threading is NEVER easy, but in
		// SmartWin it is EASIER!! ;)

		// Wee need a dummy variable since fork MUST have an argument...
		int x = 0;
		itsExitThread = fork( x, & WidgetTest1::exitApplication );
		return false;
	}

	void initAndCreate()
	{
		// Setting text of our shared object...
		sharedObject = "ABCDEFGH";

		// Creating window
		createWindow();
		setBounds( 10, 10, 1100, 800 );
		onClosing( & WidgetTest1::closeApp );
		setText( _T( "SmartWin - Threads Example" ) );

		// Creating textbox(s) with scroll bars and multiline
		WidgetTextBox::Seed textBoxCS;

		textBoxCS.style = WS_VSCROLL | WS_HSCROLL | ES_MULTILINE | WS_VISIBLE;
		textBoxCS.location = SmartWin::Rectangle( 10, 10, 400, 300 );
		text1 = createTextBox( textBoxCS );
		textBoxCS.location = SmartWin::Rectangle( 10, 310, 400, 300 );
		text2 = createTextBox( textBoxCS );

		// Creating our pause button(s)
		button1 = createButton();
		button1->setBounds( 450, 10, 550, 300 );
		button1->setText( _T( "Suspend thread (Pause thread)" ) );
		button1->onClicked( & WidgetTest1::pauseClicked1 );
		button2 = createButton();
		button2->setBounds( 450, 310, 550, 300 );
		button2->setText( _T( "Suspend thread (Pause thread)" ) );
		button2->onClicked( & WidgetTest1::pauseClicked2 );

		// Making two worker threads (that makes a total of THREE)
		itsThread1 = fork( SmartUtil::tstring( _T( "Thread 1" ) ), & WidgetTest1::newThread1 );
		itsThread2 = fork( SmartUtil::tstring( _T( "Thread 2" ) ), & WidgetTest1::newThread2 );

		int x = 0; // Dummy parameter
		itsThread3 = fork( x, newThread3 );
	}
};

int SmartWinMain( Application & app )
{
	WidgetTest1 * test = new WidgetTest1();
	test->initAndCreate();
	return app.run();
}
