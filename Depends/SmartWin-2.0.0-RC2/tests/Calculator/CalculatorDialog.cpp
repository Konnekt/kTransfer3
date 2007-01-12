// $Revision: 1.10 $
/* Copyright (c) 2005 - Thomas Hansen
 * Part of the SmartWin++ library example code
 * Permission to use under the same terms as the actual library
 * The license to the library can be found at http://smartwinlib.org
 * License for library is Open Source and BSD
 */
#include "CalculatorDialog.h"
#include "resource.h"
#include <sstream>

unsigned getNumber( const SmartUtil::tstring & stringNumber )
{
	return boost::lexical_cast< unsigned >( stringNumber );
}

SmartUtil::tstring getString( unsigned number )
{
	return boost::lexical_cast< SmartUtil::tstring >( number );
}

void CalculatorDialog::numberClicked( WidgetButtonPtr btn )
{
	if ( itsClear )
	{
		itsClear = false;
		textField->setText( _T( "" ) );
	}
	SmartUtil::tstring txt = textField->getText();
	txt += btn->getText();
	textField->setText( txt );
}

void CalculatorDialog::acClicked( WidgetButtonPtr btn )
{
	textField->setText( _T( "" ) );
	itsNumber = 0;
	itsOperation = NONE;
}

void CalculatorDialog::addSubEqualClicked( WidgetButtonPtr btn )
{
	SmartUtil::tstring txt = textField->getText();
	SmartUtil::tstring btnText = btn->getText();
	if ( _T( "+" ) == btnText )
	{
		if ( SUBTRACT == itsOperation )
		{
			textField->setText( getString( itsNumber - getNumber( textField->getText() ) ) );
		}
		else if ( ADD == itsOperation )
		{
			textField->setText( getString( itsNumber + getNumber( textField->getText() ) ) );
		}
		itsNumber = getNumber( textField->getText() );
		itsOperation = ADD;
		itsClear = true;
	}
	else if ( _T( "-" ) == btnText )
	{
		if ( SUBTRACT == itsOperation )
		{
			textField->setText( getString( itsNumber - getNumber( textField->getText() ) ) );
		}
		else if ( ADD == itsOperation )
		{
			textField->setText( getString( itsNumber + getNumber( textField->getText() ) ) );
		}
		itsNumber = getNumber( textField->getText() );
		itsOperation = SUBTRACT;
		itsClear = true;
	}
	else if ( _T( "=" ) == btnText )
	{
		if ( SUBTRACT == itsOperation )
		{
			textField->setText( getString( itsNumber - getNumber( textField->getText() ) ) );
		}
		else if ( ADD == itsOperation )
		{
			textField->setText( getString( itsNumber + getNumber( textField->getText() ) ) );
		}
		itsOperation = NONE;
	}
}

bool closed( CalculatorDialog * dlg )
{
	return true;
}

CalculatorDialog::CalculatorDialog()
	: itsNumber( 0 ),
	itsOperation( NONE ),
	itsClear( false )
{
	// Creating and initializing dialog calculator
	createDialog( IDD_DIALOG1 );

	onClosing( & closed );

	// "Morphing" the window into sight...
	animateBlend( true, 1000 );

	// Subclassing the text field on the dialog
	textField = subclassTextBox( IDC_EDIT1 );
	textField->setBorder( true ); // Must set the border since we're "morphing" into visibility with Dialog...

	// Subclassing buttons in dialog template
	WidgetButtonPtr btn1 = subclassButton( IDC_BUTTON1 );
	WidgetButtonPtr btn2 = subclassButton( IDC_BUTTON2 );
	WidgetButtonPtr btn3 = subclassButton( IDC_BUTTON3 );
	WidgetButtonPtr btn4 = subclassButton( IDC_BUTTON4 );
	WidgetButtonPtr btn5 = subclassButton( IDC_BUTTON5 );
	WidgetButtonPtr btn6 = subclassButton( IDC_BUTTON6 );
	WidgetButtonPtr btn7 = subclassButton( IDC_BUTTON7 );
	WidgetButtonPtr btn8 = subclassButton( IDC_BUTTON8 );
	WidgetButtonPtr btn9 = subclassButton( IDC_BUTTON9 );
	WidgetButtonPtr btn0 = subclassButton( IDC_BUTTON0 );
	WidgetButtonPtr btnPlus = subclassButton( IDC_BUTTON_PLUS );
	WidgetButtonPtr btnMinus = subclassButton( IDC_BUTTON_MINUS );
	WidgetButtonPtr btnEqual = subclassButton( IDC_BUTTON_EQUAL );
	WidgetButtonPtr btnAC = subclassButton( IDC_BUTTON_AC );

	// Setting callback for buttons, all have the same event handler...
	btn1->onClicked( & CalculatorDialog::numberClicked );
	btn2->onClicked( & CalculatorDialog::numberClicked );
	btn3->onClicked( & CalculatorDialog::numberClicked );
	btn4->onClicked( & CalculatorDialog::numberClicked );
	btn5->onClicked( & CalculatorDialog::numberClicked );
	btn6->onClicked( & CalculatorDialog::numberClicked );
	btn7->onClicked( & CalculatorDialog::numberClicked );
	btn8->onClicked( & CalculatorDialog::numberClicked );
	btn9->onClicked( & CalculatorDialog::numberClicked );
	btn0->onClicked( & CalculatorDialog::numberClicked );

	// Setting event handler for plus/minus...
	btnPlus->onClicked( & CalculatorDialog::addSubEqualClicked );
	btnMinus->onClicked( & CalculatorDialog::addSubEqualClicked );
	btnEqual->onClicked( & CalculatorDialog::addSubEqualClicked );

	btnAC->onClicked( & CalculatorDialog::acClicked );

	btn1->setFocus();
	//setBounds( 0, 0, 200, 300 );
}

CalculatorDialog::~CalculatorDialog()
{}
