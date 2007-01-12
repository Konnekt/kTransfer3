// $Revision: 1.7 $
// The intent here is to provide a launching window for the two modal dialogs
// contained in NumDialog.h.  One uses subclassed Widgets with positions determined
// by the Visual Studio dialog editor, and one is pure SmartWin source.
//
// This window also shows the layout capabilities from the Rectangle and Place classes.
//

#include "SmartWin.h"
#include "NumDialog.h"

using namespace SmartWin;

// The "factory" version
class ButtonToDialog
	: public WidgetFactory< WidgetWindow, ButtonToDialog >
{
private:
	WidgetButtonPtr button, purebutton, exitbut;
	WidgetStaticPtr defaultNamePrompt, nameBackPrompt, numericReturnPrompt;
	WidgetTextBoxPtr defaultName, nameBack, numericReturn;

public:
	ButtonToDialog()
	{}

	void exit_clicked( WidgetButtonPtr button )
	{
		this->close();
	}

	void clicked( WidgetButtonPtr button )
	{
		// Note stack storage!
		// Since modal dialogs does NOT get automatically destroyed you should
		// allocate them on the STACK by default and not on the HEAP!!
		NumDialog nd( this );

		// Note!
		// Since the dialog isn't constructed before calling the "createDialog" and
		// doesn't return from that function before the dialog is destroyed we need
		// some kind of "indirect" way of setting properties to the controls within
		// it... This is one solution for doing just that!
		nd.SetPrompt( _T( "Enter your name:" ), defaultName->getText() );
		nameBack->setText( _T( "" ) );

		int res = nd.createDialog( IDD_NUMIN_DIALOG ); // Blocks until the dialog is finished.
		if ( IDCANCEL == res )
		{
			numericReturn->setText( _T( "IDCANCEL" ) );
		}
		else
		{
			numericReturn->setText( _T( "IDOK" ) );
			nameBack->setText( nd.GetResponse() );
		}
	}

	void pureClicked( WidgetButtonPtr button )
	{
		PureNumDialog nd( this );
		nd.SetPrompt( _T( "Enter your name:" ), defaultName->getText() );
		nameBack->setText( _T( "" ) );

		int res = nd.createDialog(); // Blocks until the dialog is finished.

		if ( IDCANCEL == res )
		{
			numericReturn->setText( _T( "IDCANCEL" ) );
		}
		else
		{
			numericReturn->setText( _T( "IDOK" ) );
			nameBack->setText( nd.GetResponse() );
		}
	}

	void initAndCreate()
	{
		WidgetWindow::Seed windowCS;

		windowCS.location = SmartWin::Rectangle( 100, 100, 500, 170 );
		windowCS.caption = _T( "WidgetButton invokes dialog and Placement test" );
		createWindow( windowCS );
		onSized( & ButtonToDialog::isResized );

		// The inputs and outputs of the dialog
		WidgetStatic::Seed staticCS;

		staticCS.caption = _T( "Default name: " );
		staticCS.style |= ES_RIGHT; // Make the prompts right justified.
		defaultNamePrompt = createStatic( staticCS );

		defaultName = createTextBox();
		defaultName->setText( _T( "Bugs Bunny" ) );

		staticCS.caption = _T( "Name returned: " );
		nameBackPrompt = createStatic( staticCS );

		nameBack = createTextBox();

		staticCS.caption = _T( "Dialog Return: " );
		numericReturnPrompt = createStatic( staticCS );

		numericReturn = createTextBox();

		// The button bar.
		button = createButton();
		button->setText( _T( "Modal dialog from resource" ) );
		button->onClicked( & ButtonToDialog::clicked );

		purebutton = createButton();
		purebutton->setText( _T( "Pure modal dialog" ) );
		purebutton->onClicked( & ButtonToDialog::pureClicked );

		exitbut = createButton();
		exitbut->setText( _T( "Exit" ) );
		exitbut->onClicked( & ButtonToDialog::exit_clicked );

		layout(); // Notice we did not do setBounds() calls above
	}

	// There are new dimensions to the window because the user resized the window.
	void isResized( const SmartWin::WidgetSizedEventResult & sz )
	{
		layout();
		this->updateWidget();
	}

	// Common return to do layout the first time, and after each resize event.
	// The scheme is:
	//
	//  defaultNamePrompt       defaultName     \
	//  nameBackPrompt      nameBack             > Top 70%
	//  numericReturnPrompt numericReturn       /
	//  button  purebutton  exitbut                 (Bottom 30%)
	//
	void layout()
	{
		SmartWin::Rectangle r0( getClientAreaSize() );
		SmartWin::Rectangle fields( r0.top( 0.7 ) );
		SmartWin::Rectangle buttons( r0.bottom( 0.3 ) );

		SmartWin::Place p;
		p.setBoundsBorders( fields, 6, 8 ); // X, Y border between Widgets

		int rows = 3, cols = 2; // For sizing purposes, we divide into a grid.

		// The first row.
		defaultNamePrompt->setSizeAsGridPerPlace( p, rows, cols );
		defaultName->setSizeAsGridPerPlace( p, rows, cols );

		// The second row.
		// The Place class notices that we have reached the end of a row and starts the next one.
		nameBackPrompt->setSizeAsGridPerPlace( p, rows, cols );
		nameBack->setSizeAsGridPerPlace( p, rows, cols );

		numericReturnPrompt->setSizeAsGridPerPlace( p, rows, cols ); // Prompt
		numericReturn->setSizeAsGridPerPlace( p, rows, cols ); // value.

		// We will arrange the buttons with fixed widths,
		// so that different buttons can be different sizes.
		//
		p.setBoundsBorders( buttons, 12, 8 ); // X, Y border between Widgets
		SmartWin::Rectangle pos_size;

		pos_size.size.y = buttons.size.y - 16; // All buttons are the same height.

		pos_size.size.x = 200;
		p.positionToRight( pos_size ); button->setBounds( pos_size );

		pos_size.size.x = 160;
		p.positionToRight( pos_size ); purebutton->setBounds( pos_size );

		pos_size.size.x = 50;
		p.positionToRight( pos_size ); exitbut->setBounds( pos_size );
	}
};

int SmartWinMain( SmartWin::Application & app )
{
	ButtonToDialog * testWnd1 = new ButtonToDialog;
	testWnd1->initAndCreate();
	return app.run();
}
