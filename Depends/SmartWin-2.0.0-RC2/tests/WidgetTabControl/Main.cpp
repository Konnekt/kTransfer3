// $Revision: 1.9 $
/*!
  \file Main.cpp
  \brief Example application demonstrating "naive use" of a WidgetTabSheet.

  In a real world application you would probably wrap the controls inside a
  WidgetChildWindow and show and hide THAT window instead of explicitly showing and
  hiding all the controls within the tab control.
*/
#include "SmartWin.h"

using namespace SmartWin;

class WidgetTest1
	: public WidgetFactory< WidgetWindow, WidgetTest1 >
{
	WidgetTabSheetPtr sheet;

	WidgetTextBoxPtr textPage1;
	WidgetButtonPtr buttonPage2;
	WidgetComboBoxPtr comboBoxPage3;

public:
	void tabSelectionChanged( WidgetTabSheetPtr sheet )
	{
		setText( _T( "Active Tab Page was changed!" ) );
		switch ( sheet->getSelectedIndex() )
		{
		case 0 :
			textPage1->setVisible( true );
			buttonPage2->setVisible( false );
			comboBoxPage3->setVisible( false );
			break;
		case 1 :
			textPage1->setVisible( false );
			buttonPage2->setVisible( true );
			comboBoxPage3->setVisible( false );
			break;
		case 2 :
			textPage1->setVisible( false );
			buttonPage2->setVisible( false );
			comboBoxPage3->setVisible( true );
			break;
		}
	}

	bool tabSelectionChanging( WidgetTabSheetPtr sheet, unsigned sheetIndex )
	{
		setText( _T( "Active Tab Page was changed!" ) );
		switch ( sheetIndex )
		{
			case 0 :
			{
				if ( textPage1->getText().find( _T( "SmartWin" ) ) == SmartUtil::tstring::npos )
				{
					createMessageBox().show( _T( "Sorry, can't let you change sheet unless you have the word \"SmartWin\" in the text field" ) );
					return false;
				}
				return true;
			}
			default:
				return true; // Allow change...
		}
	}

	void clickedButton( WidgetButtonPtr button )
	{
		createMessageBox().show( button->getText() );
	}

	void initAndCreate()
	{
		// Creating main Widget
		WidgetWindow::Seed windowCS;

		windowCS.location = SmartWin::Rectangle( 100, 100, 480, 230 );
		windowCS.background = CreateSolidBrush( RGB( 0, 0, 125 ) );
		windowCS.caption = _T( "WidgetTabSheet - example application for tab controls!" );
		createWindow( windowCS );

		// Creating tab sheet
		WidgetTabSheet::Seed tabSheetCS;
		SmartWin::Rectangle clientArea;

		tabSheetCS.location = SmartWin::Rectangle( getClientAreaSize() ).shrink( 30l );
		sheet = createTabSheet( tabSheetCS );
		sheet->addPage( "Page 1", 0 );
		sheet->addPage( "Page 2", 1 );
		sheet->addPage( "Page 3", 2 );
		clientArea = sheet->getUsableArea();
		clientArea.pos += tabSheetCS.location.pos;

		// Creating control for page 1
		WidgetTextBox::Seed textBoxCS;

		textBoxCS.style = WS_CHILD | WS_BORDER | ES_AUTOVSCROLL | ES_SUNKEN | ES_CENTER | ES_MULTILINE;
		textBoxCS.location = clientArea.shrink( 10l );
		textBoxCS.caption = _T( "Page one, demonstrates how to use a naive TabSheet in SmartWin, just for fun try to switch a couple of "
			"times between the different tabs, then come back to this tab and try to REMOVE the words SmartWin from this text ( both places )" );
		textPage1 = createTextBox( textBoxCS );
		textPage1->setVisible( true );

		// Creating controls for page 2
		WidgetButton::Seed buttonCS;

		buttonCS.style ^= WS_VISIBLE; // instead of buttonPage2->setVisible( false );
		buttonCS.caption = _T( "Push me" );
		buttonCS.location = clientArea.shrink( 0.7 );
		buttonPage2 = createButton( buttonCS );
		buttonPage2->onClicked( & WidgetTest1::clickedButton );

		// Creating controls for page 3
		WidgetComboBox::Seed comboCS;

		comboCS.location = clientArea.shrink( 0.6 );
		comboCS.style ^= WS_VISIBLE;
		comboBoxPage3 = createComboBox( comboCS );
		comboBoxPage3->addValue( _T( "Value 1" ) );
		comboBoxPage3->addValue( _T( "Value 2" ) );

		sheet->onSelectionChanging( & WidgetTest1::tabSelectionChanging );
		sheet->onSelectionChanged( & WidgetTest1::tabSelectionChanged );
		sheet->setTabsAtBottom();
		sheet->setTabsAtBottom( false );
		sheet->setButtonStyle();
		sheet->setButtonStyle( false );
		sheet->setHotTrack();
	}
};

int SmartWinMain( Application & app )
{
	WidgetTest1 * test = new WidgetTest1();
	test->initAndCreate();
	return app.run();
}
