// $Revision: 1.8 $
/* Copyright (c) 2005 - Thomas Hansen
 * Part of the SmartWin++ library example code
 * Permission to use under the same terms as the actual library
 * The license to the library can be found at http://smartwinlib.org
 * License for library is Open Source and BSD
 */
#include "SmartWin.h"
using namespace SmartWin;

class HelloWinClass
			: public WidgetFactory< WidgetWindow, HelloWinClass >
{
private:
	WidgetMenuPtr itsMainMenu;
	WidgetButtonPtr itsButton;
	WidgetCheckBoxPtr itsCheckBox;

public:
	void menuSayHello( WidgetMenuPtr menu, unsigned item )
	{
		createMessageBox().show( _T( "Hello !" ), menu->getText( item ) );
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

	void initAndCreate()
	{
		createWindow();
		setText( _T( "Hello SmartWin" ) ); // Title

		SmartWin::Rectangle desktop( getDesktopSize() );
		setBounds( desktop.top( 0.2 ).left( 0.3 ) );

		itsButton = createButton();
		itsButton->setText( _T( "Hello from a button" ) );
		itsButton->onClicked( & HelloWinClass::buttonClicked );
		itsButton->setBounds( sw::Rectangle( Point( 200, 30 ) ) );

		itsCheckBox = createCheckBox();
		itsCheckBox->setText( _T( "&Global" ) );
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
	HelloWinClass * testHello = new HelloWinClass;
	testHello->initAndCreate();
	return app.run();
}
