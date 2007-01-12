// $Revision: 1.9 $
/*!
  \file Main.cpp
  \brief Unit test for WidgetTextBox.

  It tries to instantiate all different forms of WidgetTextBox and it also tries to use all functions.
*/

#include "SmartWin.h"
#include <vector>
#include <assert.h>
using namespace SmartWin;

// The "factory" version
class WidgetTest1
	: public WidgetFactory< WidgetWindow, WidgetTest1 >
{
public:
	WidgetTest1()
	{}

	void selectionChanged( WidgetComboBoxPtr comboBox )
	{
		createMessageBox().show( _T( "New selection: " ) + comboBox->getSelectedValue() );
	}

	void initAndCreate()
	{
		WidgetWindow::Seed windowCS;

		// Uncomment the following code to get the blue background shown in the
		// screenshoots. Currently, only the background of the main window can be
		// changed using Seed.
		
		// windowCS.background = CreateSolidBrush( RGB( 0, 0, 125 ) );
		windowCS.location = SmartWin::Rectangle( 100, 100, 370, 250 );
		windowCS.caption = _T( "WidgetComboBox compile and execute test" );
		createWindow( windowCS );

		WidgetComboBox::Seed widgetComboBoxCS;

		widgetComboBoxCS.location = SmartWin::Rectangle( 50, 50, 200, 400 );

		WidgetComboBoxPtr comboBox = createComboBox( widgetComboBoxCS );

		comboBox->addValue( _T( "Value1" ) );
		comboBox->addValue( _T( "Value2" ) );
		comboBox->addValue( _T( "Value3" ) );
		comboBox->addValue( _T( "Value4" ) );
		comboBox->addValue( _T( "Value5" ) );
		comboBox->addValue( _T( "Value6" ) );
		comboBox->addValue( _T( "Value7" ) );
		comboBox->setSelectedIndex( 4 );
		assert( comboBox->getVisible() );
		assert( comboBox->getSelectedValue() == _T( "Value5" ) );
		assert( comboBox->getSelectedIndex() == 4 );
		assert( comboBox->getCount() == 7 );
		assert( comboBox->getValue( 0 ) == _T( "Value1" ) );
		assert( comboBox->getValue( 6 ) == _T( "Value7" ) );

		comboBox->onSelectionChanged( & WidgetTest1::selectionChanged );

		WidgetComboBoxPtr comboBox2 = createComboBox();
		comboBox2->setBounds( SmartWin::Point( 100, 100 ), SmartWin::Point( 200, 400 ) );
		comboBox2->addValue( _T( "Disabled Value1" ) );
		comboBox2->addValue( _T( "Disabled Value2" ) );
		comboBox2->addValue( _T( "Disabled Value3" ) );
		comboBox2->addValue( _T( "Disabled Value4" ) );
		comboBox2->addValue( _T( "Disabled Value5" ) );
		comboBox2->addValue( _T( "Disabled Value6" ) );
		comboBox2->addValue( _T( "Disabled Value7" ) );
		comboBox2->setSelectedIndex( 4 );
		comboBox2->setEnabled( false );
		assert( !comboBox2->getEnabled() );

		comboBox->setFocus();
	}
};

int SmartWinMain( Application & app )
{
	// Widget (Factory) version
	WidgetTest1 * testWnd1 = new WidgetTest1;
	testWnd1->initAndCreate();
	WidgetTest1::WidgetComboBox * freeBox = testWnd1->createComboBox();
	freeBox->setBounds( 150, 150, 100, 100 );
	freeBox->addValue( _T( "TestingFree1" ) );
	freeBox->addValue( _T( "TestingFree2" ) );
	freeBox->setVisible( false );
	assert( freeBox->getVisible() == false );
	freeBox->setVisible( true );
	assert( freeBox->getVisible() == true );
	return app.run();
}
