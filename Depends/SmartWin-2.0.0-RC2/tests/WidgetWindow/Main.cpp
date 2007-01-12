// $Revision: 1.12 $
/*!
  \file Main.cpp
  \brief WidgetWindow sample program

   It shows how to intercept closing and focus messages. It also how to make a
   docking window which can be tied to another window or also free at runtime.
*/
#include "SmartWin.h"
using namespace SmartWin;

// The "factory" version
class WidgetTest1
	: public WidgetFactory< WidgetWindow, WidgetTest1 >
{
public:
	WidgetTest1()
		: Widget()
	{}

	bool shouldClose()
	{
		if ( createMessageBox().show( _T( "Should I close" ), _T( "Closing question" ), WidgetMessageBox::BOX_OKCANCEL )
			== WidgetMessageBox::RETBOX_OK )
			return true;
		return false;
	}

	void createMainWidget( const SmartWin::Seed & cs )
	{
		animateCollapse( false, 500 );
		animateCollapse( true, 500 );

		// Member version
		onClosing( & WidgetTest1::shouldClose );
		onKillFocus( & WidgetTest1::killFocus );
		onFocus( & WidgetTest1::focus );

		setMinimizeBox( false );
		setMaximizeBox( false );
	}

	void killFocus()
	{
		setText( _T( "No Focus" ) );
	}

	void focus()
	{
		setText( _T( "HAS Focus, Hooray!" ) );
	}

	void initAndCreate()
	{
		WidgetWindow::Seed cs;

		cs.background = CreateSolidBrush( RGB( 0, 0, 125 ) );
		cs.caption = _T( "WidgetWindow ( factory ) compile and execute test" );
		onCreate( & WidgetTest1::createMainWidget );
		createWindow( cs );
		this->setBounds( 0, 0, 400, 200 );
	}
};

// BUG : Under WINE, it is never called.
// Under Windows, it is called and destroys the docked window.
//
// Destroy the docked window
// RETURN TRUE if we need to look at more windows.
//
BOOL CALLBACK EnumChildProc( HWND hwndChild, LPARAM lParam )
{
	TCHAR title[99];
	GetWindowText( hwndChild, title, 99 );
	if ( 0 == _tcscmp( title, _T( "Dockable WidgetWindow" ) ) )
	{
		DestroyWindow( hwndChild );
		return FALSE; // No more child windows
	}

	 return TRUE; // keep trying
}

// The "non - factory" version
class WidgetTest2
	: public WidgetWindow< WidgetTest2 >
{
public:
	WidgetTest2()
		: Widget( 0 )
	{}

	// Note!
	// Static!
	static bool shouldClose( WidgetTest2 * This )
	{
		// Note also that this is the "non factory" version which means we can't call the createMessageBox on the This pointer
		typedef WidgetMessageBox< WidgetTest2 > MessageBox;
		MessageBox box( This );
		if ( box.show( _T( "Should I close" ), _T( "Closing question" ), MessageBox::BOX_OKCANCEL )
			== MessageBox::RETBOX_OK )
		{
			EnumChildWindows( This->handle(), EnumChildProc, ( LPARAM ) 0 );
			return true;
		}
		return false;
	}

	static void createMainWidget( WidgetTest2 * ptr, const SmartWin::Seed & cs )
	{
		ptr->animateBlend( false, 500 );
		ptr->animateBlend( true, 500 );

		// Global/static version
		ptr->onClosing( & WidgetTest2::shouldClose );
		ptr->onKillFocus( & WidgetTest2::killFocus );
		ptr->onFocus( & WidgetTest2::focus );
	}

	static void killFocus( WidgetTest2 * ptr )
	{
		ptr->setText( _T( "No Focus" ) );
	}

	static void focus( WidgetTest2 * ptr )
	{
		ptr->setText( _T( "HAS Focus, Jippie!" ) );
	}

	void initAndCreate()
	{
		Seed cs;

		cs.background = CreateSolidBrush( RGB( 125, 125, 0 ) );
		cs.caption = _T( "WidgetWindow ( non - factory ) compile and execute test" );
		cs.location = SmartWin::Rectangle( 401, 200, 400, 200 );
		onCreate( & WidgetTest2::createMainWidget );
		createWindow( cs );
	}
};

// This class has the ability to be docked and undocked from another window.
//
class DockingWidget
	: public WidgetFactory< WidgetWindow, DockingWidget >
{
private:
		WidgetButtonPtr itsButton;

public:
	DockingWidget( Widget * parent )
		: Widget( parent, NULL, true ) // Accept the passed parent window
	{}

	void dock( WidgetButtonPtr but )
	{
		addRemoveStyle( WS_POPUP, false ); // Means take away WS_POPUP
		addRemoveStyle( WS_CHILD, true ); // and add WS_CHILD
		::SetParent( handle(), getParent()->handle() );
		setBounds( 10, 10, 170, 80 ); // Must resize for coordinates inside the parent.
		itsButton->setText( _T( "Undock" ) );
		itsButton->onClicked( & DockingWidget::undock );
	}

	void undock( WidgetButtonPtr but )
	{
		addRemoveStyle( WS_CHILD, false );
		addRemoveStyle( WS_POPUP, true );
		::SetParent( handle(), NULL );

		setBounds( 601, 0, 200, 200 );
		itsButton->setText( _T( "Dock" ) );
		itsButton->onClicked( & DockingWidget::dock );
	}

	void initAndCreate()
	{
		Seed cs;
		cs.exStyle = WS_EX_TOOLWINDOW;
		cs.style = WS_POPUP | WS_SYSMENU | WS_THICKFRAME | WS_CAPTION | WS_VISIBLE;

		createWindow( cs );
		setText( _T( "Dockable WidgetWindow" ) );
		setBounds( 601, 0, 200, 200 );
		itsButton = createButton();
		itsButton->setBounds( 0, 0, 80, 30 );
		itsButton->onClicked( & DockingWidget::dock );
		itsButton->setText( _T( "Dock" ) );
	}
};

// Unit test for WidgetSaveFile, tries to instantiate all different forms of WidgetSaveFile and also tries to use all functions
int SmartWinMain( SmartWin::Application & app )
{
	// Widget (Factory) version
	WidgetTest1 * testWnd1 = new WidgetTest1;
	WidgetTest2 * testWnd2 = new WidgetTest2;

	testWnd1->initAndCreate();
	testWnd2->initAndCreate();

	DockingWidget * testWnd3 = new DockingWidget( testWnd2 );
	testWnd3->initAndCreate();

	testWnd1->setFocus();
	return app.run();
}
