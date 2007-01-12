// $Revision: 1.18 $
/*!
  \file Main.cpp
  \brief Unit test for WidgetToolbar.

  It tries to instantiate all different forms of WidgetToolbar and also tries to use
  all functions.
*/

#include "SmartWin.h"
#include "resource.h"
#include <vector>
#include <assert.h>
using namespace SmartWin;

// The "factory" version
class WidgetTest1
	: public WidgetFactory< WidgetWindow, WidgetTest1 >
{
	WidgetChildWindowPtr docking;
	WidgetCoolbarPtr coolbar;
	WidgetComboBoxPtr comboBox;
	WidgetTextBoxPtr textBox;
	WidgetButtonPtr button;
	WidgetTreeViewPtr tree;
	WidgetCheckBoxPtr check;
	WidgetSpinnerPtr spinner;
	WidgetGroupBoxPtr groupBox;
	WidgetMenuPtr menu;

public:
	WidgetTest1()
	{}

	// A couple of nifty events...
	void ComboBoxSelectionChanged( WidgetCoolbar::WidgetComboBoxPtr box )
	{
		setText( box->getSelectedValue() );
	}

	void textBoxUpdated( WidgetTextBoxPtr txt )
	{
		setText( txt->getText() );
	}

	// Needed to get the refreshing of the Coolbar when window sizes
	// If ommitted Coolbar WON'T resize together with main container Widget...
	void sized( const sw::WidgetSizedEventResult & sz )
	{
		DWORD style = GetWindowStyle( docking->handle() );
		if ( ( style & WS_CHILD ) != 0 )
		{
			sw::Rectangle rect = getBounds();
			docking->setBounds( 0, 0, rect.size.x, 450 );
			coolbar->refresh();
		}
	}

	void treeSelectionChanged( WidgetTreeViewPtr tree )
	{
		setText( tree->getSelectedItemText() );
	}

	void buttonClicked( WidgetButtonPtr btn )
	{
		DWORD style = GetWindowStyle( docking->handle() );
		if ( ( style & WS_CHILD ) == 0 )
		{
			docking->addRemoveStyle( WS_CHILD, true );
			docking->addRemoveStyle( WS_POPUP, false );
			docking->addRemoveStyle( WS_CAPTION, false );
			::SetParent( docking->handle(), docking->getParent()->handle() );
			sw::Rectangle rect = getBounds();
			docking->setBounds( 0, 0, rect.size.x, 450 );
			coolbar->refresh();
		}
		else
		{
			docking->addRemoveStyle( WS_CHILD, false );
			docking->addRemoveStyle( WS_POPUP, true );
			docking->addRemoveStyle( WS_CAPTION, true );
			::SetParent( docking->handle(), NULL );
		}
	}

	void menuEventHandler( WidgetMenuPtr menu, unsigned item )
	{
		setText( _T( "Menu clicked" ) );
	}

	void initAndCreate()
	{
		WidgetWindow::Seed windowCS;

		windowCS.location = SmartWin::Rectangle( 120, 120, 700, 450 );
		windowCS.background = CreateSolidBrush( RGB( 0, 0, 125 ) );
		windowCS.caption = _T( "WidgetToolbar compile and execute test" );
		createWindow( windowCS );

		// Creates the actual DOCKING window...
		WidgetChildWindow::Seed widgetChildWindowSeed;

		widgetChildWindowSeed.style |= CS_NOCLOSE;
		widgetChildWindowSeed.location = SmartWin::Rectangle( getClientAreaSize() ).top();
		docking = createWidgetChildWindow( widgetChildWindowSeed );

		// Then create our Coolbar object, note it's the child of our WidgetChildWindow to make it possible to dock and undock...
		coolbar = docking->createCoolbar();

		FontPtr font = createFont( _T( "Comic Sans MS" ), 12, 6, 1 );
		coolbar->setFont( font );

		// Then create a couple of Widgets to stuff into it
		// ComboBox first
		WidgetComboBox::Seed comboSeed;

		comboSeed.location.size = SmartWin::Point( 180, 60 );
		comboBox = coolbar->sow( comboSeed, _T( "ComboBox ( try clicking text )" ) );
		comboBox->addValue( _T( "Value 1" ) );
		comboBox->addValue( _T( "Value 2" ) );
		comboBox->addValue( _T( "Value 3" ) );
		comboBox->onSelectionChanged( & WidgetTest1::ComboBoxSelectionChanged );

		// Then a TextBox Widget
		WidgetTextBox::Seed textBoxSeed;

		textBoxSeed.location.size = SmartWin::Point( 200, 40 );
		textBoxSeed.caption = _T( "Type into me!" );
		textBox = coolbar->sow( textBoxSeed, _T( "Text: " ) );
		textBox->onUpdate( & WidgetTest1::textBoxUpdated );

		//// Then a "normal" button
		WidgetButton::Seed buttonSeed;

		buttonSeed.location.size = SmartWin::Point( 140, 40 );
		button = coolbar->sow( buttonSeed, _T( "Button: " ) );
		button->setText( _T( "Dock/Undock \"toolbar\"" ) );
		button->onClicked( & WidgetTest1::buttonClicked );

		//// Then a treeview Widget
		WidgetTreeView::Seed treeViewSeed;

		treeViewSeed.location.size = SmartWin::Point( 200, 40 );
		tree = coolbar->sow( treeViewSeed, _T( "TreeView: " ) );
		sw::TreeViewNode alpha = tree->insertNode( _T( "Alpha" ) );
		tree->insertNode( _T( "Alpha 1'st child" ), alpha );
		tree->insertNode( _T( "Alpha 2'nd child" ), alpha );
		tree->onSelectionChanged( & WidgetTest1::treeSelectionChanged );

		//// Then a "checkbox" widget
		WidgetCheckBox::Seed checkBoxSeed;

		checkBoxSeed.location.size = SmartWin::Point( 200, 40 );
		check = coolbar->sow( checkBoxSeed, _T( "Check button: " ) );
		check->setText( _T( "Check me!" ) );

		//// Then a spinner widget
		WidgetSpinner::Seed spinnerSeed;

		spinnerSeed.location.size = SmartWin::Point( 200, 40 );
		spinner = coolbar->sow( spinnerSeed, _T( "Spinner: " ) );

		//// Then a GroupBox widget
		WidgetGroupBox::Seed groupBoxSeed;

		groupBoxSeed.location.size = SmartWin::Point( 200, 40 );
		groupBox = coolbar->sow( groupBoxSeed, _T( "Radio buttons" ) );

		//// Then adding up two RadioButtons into the GroupBox
		//// TODO: Doesn't work...
		//WidgetRadioButton::Seed radioButtonSeed;

		//groupBoxSeed.location.size = SmartWin::Point( 200, 40 );
		//radioButtonSeed.caption = _T("Uno");
		//WidgetRadioButtonPtr radio1 = coolbar->sow( groupBox, radioButtonSeed );
		//radioButtonSeed.caption = _T("Dos");
		//WidgetRadioButtonPtr radio2 = coolbar->sow( groupBox, radioButtonSeed );

		// DateTimePicker
		WidgetDateTimePicker::Seed widgetDateTimePickerSeed;

		widgetDateTimePickerSeed.location.size = SmartWin::Point( 200, 80 );
		WidgetDateTimePickerPtr dateTime = coolbar->sow( widgetDateTimePickerSeed, _T( "DateTime Picker: " ) );

		//// Menu
		//// TODO: Doesn't work...
		//menu = coolbar->createMenu( 100, 20 );
		//WidgetMenuPtr file = menu->appendPopup( _T("&File") );
		//file->appendItem( 1, _T("&Load"), &WidgetTest1::menuEventHandler );
		//file->appendItem( 2, _T("&Save"), &WidgetTest1::menuEventHandler );

		onSized( & WidgetTest1::sized );
	}
};

int SmartWinMain( Application & app )
{
	// Widget (Factory) version
	WidgetTest1 * testWnd1 = new WidgetTest1;
	testWnd1->initAndCreate();
	return app.run();
}
