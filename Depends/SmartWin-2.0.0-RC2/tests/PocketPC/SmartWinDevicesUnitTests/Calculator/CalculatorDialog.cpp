// $Revision: 1.7 $

#include "CalculatorDialog.h"
#include "resourcecpp.h"
#include <sstream>

unsigned getNumber( const SmartUtil::tstring & string )
{
	std::basic_stringstream< TCHAR > str;
	str << string;
	str.seekg( 0 );
	int retVal;
	str >> retVal;
	return retVal;
}

SmartUtil::tstring getString( unsigned number )
{
	std::basic_stringstream< TCHAR > str;
	str << number;
	str.seekg( 0 );
	SmartUtil::tstring retVal;
	str >> retVal;
	return retVal;
}

void CalculatorDialog::numberClicked( WidgetButtonPtr btn )
{
	if ( clear )
	{
		clear = false;
		textField->setText( _T( "" ) );
	}
	SmartUtil::tstring txt = textField->getText();
	txt += btn->getText();
	textField->setText( txt );
}

void CalculatorDialog::acClicked( WidgetButtonPtr btn )
{
	textField->setText( _T( "" ) );
	number = 0;
	operation = NONE;
}

void CalculatorDialog::addSubEqualClicked( WidgetButtonPtr btn )
{
	SmartUtil::tstring txt = textField->getText();
	SmartUtil::tstring btnText = btn->getText();
	if ( _T( "+" ) == btnText )
	{
		if ( SUBTRACT == operation )
		{
			textField->setText( getString( number - getNumber( textField->getText() ) ) );
		}
		else if ( ADD == operation )
		{
			textField->setText( getString( number + getNumber( textField->getText() ) ) );
		}
		number = getNumber( textField->getText() );
		operation = ADD;
		clear = true;
	}
	else if ( _T( "-" ) == btnText )
	{
		if ( SUBTRACT == operation )
		{
			textField->setText( getString( number - getNumber( textField->getText() ) ) );
		}
		else if ( ADD == operation )
		{
			textField->setText( getString( number + getNumber( textField->getText() ) ) );
		}
		number = getNumber( textField->getText() );
		operation = SUBTRACT;
		clear = true;
	}
	else if ( _T( "=" ) == btnText )
	{
		if ( SUBTRACT == operation )
		{
			textField->setText( getString( number - getNumber( textField->getText() ) ) );
		}
		else if ( ADD == operation )
		{
			textField->setText( getString( number + getNumber( textField->getText() ) ) );
		}
		operation = NONE;
	}
}

bool closed( CalculatorDialog * dlg )
{
	return true;
}

CalculatorDialog::CalculatorDialog()
	: number( 0 ),
	operation( NONE ),
	clear( false )
{
	// Creating and initializing dialog calculator
	createDialog( IDD_DIALOG1 );

	onClosing( & closed );

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
	setBounds( 20, 40, 170, 210 );
	updateWidget();
}

CalculatorDialog::~CalculatorDialog()
{
}
