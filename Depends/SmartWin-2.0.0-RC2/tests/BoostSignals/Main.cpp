// $Revision: 1.2 $
/* Copyright (c) 2005 - Thomas Hansen
 * Part of the SmartWin++ library example code
 * Permission to use under the same terms as the actual library
 * The license to the library can be found at http://smartwinlib.org
 * License for library is Open Source and BSD
 */
#include "SmartWin.h"
#include <vector>
using namespace SmartWin;
using namespace SmartUtil;
using namespace std;

/*
 * This class implements the basic operations the program should carry out.
 * This class provides an "entry function" which creates two waitable events
 * (that also can be thread handles, console inputs, mutexes etc..).
 * The Tasks class provides two callback handlers (slots) which are called by the 
 * SmartWin::Application object if Windows has signalled those appropriated HANDLEs 
 * of the waitable events.
 * Tasks inherits from boost::signals::trackable, which means that boost::signal
 * objects bound to Tasks, can track the lifetime of Tasks, i.e. if the Tasks object 
 * had somehow been destroyed before a signal occurs, the slot relating to the Tasks
 * object is not called.
 */
class Tasks : public boost::signals::trackable
{
	typedef boost::signal < void() > TypeSignal;
	typedef boost::shared_ptr< TypeSignal > TypeSignalPtr;

private:
	// the signals we wish to associate with our events
	TypeSignalPtr itsPtrSignalA;
	TypeSignalPtr itsPtrSignalB;

	// the HANDLE values we should pass to Windows
	HANDLE itsHandleProcess;
	HANDLE itsHandleFileChange;

public:
	/*
	 * Constructs a Task object, defaults its data memebers and stores a reference
	 * of the programs SmartWin::Application object
	 */
	Tasks()
		: itsPtrSignalA( new TypeSignal() ),
		itsPtrSignalB( new TypeSignal() ),
		itsHandleProcess( INVALID_HANDLE_VALUE ),
		itsHandleFileChange( INVALID_HANDLE_VALUE )
	{}

	/*
	 * frees used HANDLEs
	 */
	~Tasks()
	{
		if ( itsHandleProcess != INVALID_HANDLE_VALUE )
		{
			::CloseHandle( itsHandleProcess );
		}
		if ( itsHandleFileChange != INVALID_HANDLE_VALUE )
		{
			::FindCloseChangeNotification( itsHandleFileChange );
		}
	}

	// This "entry function" performs operations which return waitable HANDLEs
	/*
	 * A "notepad.exe" process is invoked (note this is bad on Linux!).
	 * @param bWatchCurrentPath if true we tell Windows to watch in the application
	 * path for file changes
	 */
	void spawnTasks( bool bWatchCurrentPath )
	{
		// Event A:
		// spawn a child process

		// assemble the program location for "notepad.exe"
		vector< TCHAR > vc( ::GetWindowsDirectory( 0, 0 ) );
		size_t cChars = static_cast< size_t >( ::GetWindowsDirectory( & vc[0], static_cast< UINT >( vc.size() ) ) );
		tstring strProgram( & vc[0], cChars );
		strProgram += _T( "\\notepad.exe" );

		// launch the program
		SHELLEXECUTEINFO strctExeInfo;
		::ZeroMemory( & strctExeInfo, sizeof( SHELLEXECUTEINFO ) );
		strctExeInfo.cbSize = sizeof( SHELLEXECUTEINFO );
		strctExeInfo.hwnd = ::GetDesktopWindow();
		strctExeInfo.lpVerb = _T( "open" );
		strctExeInfo.lpFile = strProgram.c_str();
		strctExeInfo.lpParameters = NULL;
		strctExeInfo.lpDirectory = NULL;
		strctExeInfo.nShow = SW_SHOWNORMAL;
		strctExeInfo.fMask = SEE_MASK_NOCLOSEPROCESS;

		if ( ::ShellExecuteEx( & strctExeInfo ) )
		{
			// keep the process handle to close it when finished
			itsHandleProcess = strctExeInfo.hProcess;

			// specify the event handler
			itsPtrSignalA->connect( boost::bind( & Tasks::cleanHandle, this ) );

			// register the wait event in the application
			Application::instance().addWaitEvent( strctExeInfo.hProcess, itsPtrSignalA );
		}

		if ( bWatchCurrentPath )
		{
			// Event B:
			// we want to be notified if a file has changed in the current directory
			itsHandleFileChange = ::FindFirstChangeNotification(
				Application::instance().getModulePath().c_str(),
				FALSE, //bSubTree
				FILE_NOTIFY_CHANGE_FILE_NAME );

			// specify the event handler
			itsPtrSignalB->connect( boost::bind( & Tasks::onFileChanged, this ) );

			// register the wait event in the application
			Application::instance().addWaitEvent( itsHandleFileChange, itsPtrSignalB );
		}
	}

	// this is the slot for the "child process terminated" event
	void cleanHandle()
	{
		// we do not want to wait any longer for this event
		Application::instance().removeWaitEvent( itsHandleProcess );

		::CloseHandle( itsHandleProcess );
		itsHandleProcess = INVALID_HANDLE_VALUE;

		::MessageBox( NULL, _T( "The process was closed." ), _T( "Event A" ), MB_OK );
	}

	// this is the slot for the event indicating a file change
	void onFileChanged()
	{
		// we do not want to wait for another file change
		Application::instance().removeWaitEvent( itsHandleFileChange );

		::FindCloseChangeNotification( itsHandleFileChange );
		itsHandleFileChange = INVALID_HANDLE_VALUE;

		WidgetMessageBoxFree tmp;
		tmp.show( _T( "A file was changed." ), _T( "Event B" ) );
	}
};

/*
 * taken from the SmartWin/UnitTests/HelloSmartWinWorld example
 */
class HelloWinClass : public WidgetFactory< WidgetWindow, HelloWinClass >
{
private:
	WidgetMenuPtr itsMainMenu;
	WidgetButtonPtr itsButton;
	WidgetCheckBoxPtr itsCheckBox;

	Tasks itsPtrTasks;

public:
	void menuSayHello( WidgetMenuPtr menu, unsigned item )
	{
		createMessageBox().show( _T( "Hello !" ), menu->getText( item ) );
	}

	void menuClose( WidgetMenuPtr menu, unsigned item )
	{
		close( true );
	}

	void buttonClicked( WidgetButtonPtr button )
	{
		itsPtrTasks.spawnTasks( itsCheckBox->getChecked() );
	}

	void initAndCreate()
	{
		createWindow();
		setText( _T( "Hello SmartWin" ) ); // Title

		SmartWin::Rectangle desktop( getDesktopSize() );
		setBounds( desktop.top( 0.2 ).left( 0.3 ) );

		itsButton = createButton();
		itsButton->setText( _T( "Launch Notepad button" ) );
		itsButton->onClicked( & HelloWinClass::buttonClicked );
		itsButton->setBounds( sw::Rectangle( Point( 200, 30 ) ) );

		itsCheckBox = createCheckBox();
		itsCheckBox->setText( _T( "Watch the current path" ) );
		itsCheckBox->setBounds( sw::Rectangle( Point( 200, 30 ) ) );

		// Creating main menu
		itsMainMenu = createMenu();
		WidgetMenuPtr file = itsMainMenu->appendPopup( _T( "&MenuCommands" ) );
		int m = 1;
		file->appendItem( m++, _T( "Hello from the menu" ), & HelloWinClass::menuSayHello );
		file->appendItem( m++, _T( "Close" ), & HelloWinClass::menuClose );

#ifndef WINCE
		itsMainMenu->attach( this );
#endif
		layout();
		onSized( & HelloWinClass::isResized );
	}

	void isResized( const WidgetSizedEventResult & sz )
	{
		layout();
	}

	void layout()
	{
		SmartWin::Place p;
		SmartWin::Rectangle r( getClientAreaSize() );
		p.setBoundsBorders( r, 4, 4 );

		itsCheckBox->setPositionPerPlace( p );
		itsButton->setPositionPerPlace( p );
	}
};

int SmartWinMain( Application & app )
{
	HelloWinClass * testHello = new HelloWinClass();
	testHello->initAndCreate();
	return app.run();
}
