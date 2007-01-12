// $Revision: 1.3 $
// Pretty much the standard HelloWinWorld sample window, but I added a InDialog
// menu function. There is nothing DLL specific in this file.
//
class HelloDllClass
	: public WidgetFactory< WidgetWindow, HelloDllClass >
{
private:
	WidgetMenuPtr itsMainMenu;
	WidgetButtonPtr itsButton;
	WidgetCheckBoxPtr itsCheckBox;

	int itsInteger; int * outIntPtr;
	SmartUtil::tstring itsString; char * outStrPtr;

public:

	void menuSayHello( WidgetMenuPtr menu, unsigned item )
	{
		createMessageBox().show( _T( "Hello !" ), menu->getText( item ) );
	}

	void menuInDialog( WidgetMenuPtr menu, unsigned item )
	{
		BoundedLong bl( 1994, 1900, 2050 );
		float float1 = ( float ) 98.6;
		double doub1 = 876.543;

		PasswordString pass1; // or ( 8, "" )
		bool bool1 = true;

		ChoiceString cs( "scissors|rock|paper", 2 );

		FileString loadFs( "C:\\path\\gumbo.txt" );
		loadFs.addFilter( "JPG files", "*.jpg" );
		FileString saveFs( "", false );
		saveFs.addFilter( "JPG files", "*.jpg" );

		SYSTEMTIME bookDue;
		::GetSystemTime( & bookDue );

		COLORREF background = RGB( 20, 40, 80 );

		bool c;

		InDialog myInDialog( this, "First InDialog Attempt !" );
		if ( IDCANCEL == myInDialog
		.add( _T( "Input file ..." ), & loadFs )
		.add( _T( "Output file ..." ), & saveFs )
		.add( _T( "Temperature:" ), & itsInteger )
		.add( _T( "Average temperature:" ), & float1 )
		.add( _T( "Average weight:" ), & doub1 )
		.add( _T( "Birth year (1900-2050)" ), & bl )
		.add( _T( "Fries ?" ), & bool1 )
		.add( _T( "Say:" ), & itsString )
		.add( _T( "Password:" ), & pass1 )
		.add( _T( "Throw:" ), & cs )
		.add( _T( "Book due:" ), & bookDue )
		.add( _T( "Background color ..." ), & background )
		.createDialog() ) return;
	}

	void menuClose( WidgetMenuPtr menu, unsigned item )
	{
		close();
	}

	void buttonClicked( WidgetButtonPtr button )
	{
		if ( itsCheckBox->getChecked() )
		{
			createMessageBox().show( _T( "Hello World!" ), button->getText() );
		}
		else
		{
			createMessageBox().show( _T( "Hello !" ), button->getText() );
		}
	}

	// We pass in the return parameters, save them, and fill them upon exit of the
	// window.
	void initAndCreate( int * numb, char str[] )
	{
		outIntPtr = numb; // Save return parameters for later.
		outStrPtr = str;

		createWindow();
		setText( _T( "Hello SmartWin" ) ); // Title
		itsInteger = - 8;
		itsString = "hello via SmartWin++ window";

		SmartWin::Rectangle desktop( getDesktopSize() );
		setBounds( desktop.top( 0.2 ).left( 0.3 ) );

		itsButton = createButton();
		itsButton->setText( _T( "Hello from a button" ) );
		itsButton->onClicked( & HelloDllClass::buttonClicked );
		itsButton->setBounds( sw::Rectangle( Point( 200, 30 ) ) );

		itsCheckBox = createCheckBox();
		itsCheckBox->setText( _T( "&Global" ) );
		itsCheckBox->setBounds( sw::Rectangle( Point( 200, 30 ) ) );

		// Creating main menu
		itsMainMenu = createMenu();
		WidgetMenuPtr file = itsMainMenu->appendPopup( _T( "&MenuCommands" ) );
		int m = 1;
		file->appendItem( m++, _T( "Hello from the menu" ), & HelloDllClass::menuSayHello );
		file->appendItem( m++, _T( "Modal dialog" ), & HelloDllClass::menuInDialog );
		file->appendItem( m++, _T( "Close" ), & HelloDllClass::menuClose );

#ifndef WINCE
		itsMainMenu->attach( this );
#endif
		layout();
		onSized( & HelloDllClass::isResized );
		this->onClosing( & HelloDllClass::isClosing );
	}

	bool isClosing()
	{
		* outIntPtr = itsInteger;
		strcpy( outStrPtr, itsString.c_str() );

		return true;
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
