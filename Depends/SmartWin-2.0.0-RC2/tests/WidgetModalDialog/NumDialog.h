// $Revision: 1.9 $
#ifndef NUMDIALOG_H
#define NUMDIALOG_H

#include "SmartWin.h"

//#define BUG
#ifdef BUG
#include "io/InDialog.h"        // Input some integers, strings, and booleans.
#endif

#include "resource.h"

using namespace SmartWin;

/*
Provides a general purpose input dialog.

USAGE:
		NumDialog nd( pointerToWidgetWhichShouldBeParent );
		nd.SetPrompt( _T("Enter your name:"), _T("john modal dialog")  );

		INT_PTR res= nd.createDialog( IDD_NUMIN_DIALOG );   // Blocks until the dialog is finished.

		if ( ID_CANCEL == res )
			setText( _T("Canceled") );
		else
			setText( nd.GetResponse() );

		Note especially one BIG difference with WidgetModalDialogs!!
		They are NOT selfdestructive like mostly all other Widgets are!!!
		This means that you must yourself make sure they're destroyed!
		But since they are in nature "procedural" meaning you don't return from the createDialog
		this is hardly a problem and can be bypassed by just creating them directly on the stack!
		But be aware since this is "counter intuitive" compared to all other Widgets!!

CONSTRAINTS ON THE DIALOG CLASS:
	1) The constructor should prepare an "onInitDialog( initDialog )" so the Widgets
	   are created between the dialog creation and visability.

	2) EndDialog( retv ); should be called to dismiss the dialog.

	3) If you want control of the Dialog you must handle the "onClosing" event and return ** false ** but rather
	   call EndDialog INSIDE the onClosing event handler!

FURTHER QUESTIONS:
	1) Can modal dialogs contain more modal dialogs ?
	   A) Yup! As long as they're not "System Modal", but even that works too, just looks a bit odd!!
	2) What if you want to create a modal dialog without a resource ? 
	   A) See PureNumDialog below.

	2) EndDialog( retv ); should be called to dismiss the dialog.

UNIMPLEMETED IMPROVEMENTS:
	1) Parent window must deactivate itself just before calling CreateDialog
*/

class NumDialog
	: public WidgetFactory< WidgetModalDialog, NumDialog, MessageMapPolicyModalDialogWidget >
{
public:
	void SetPrompt( SmartUtil::tstring inPromptTxt, SmartUtil::tstring inInitialTxt )
	{
		itsPromptTxt = inPromptTxt;
		itsResultTxt = inInitialTxt;
	}

	SmartUtil::tstring GetResponse()
	{
		return( itsResultTxt );
	}

	NumDialog( Widget * parent )
		: Widget( parent )
	{
		onInitDialog( & NumDialog::initDialog );
	}

	~NumDialog()
	{
	}

private:
	WidgetTextBoxPtr itsTextBox;
	WidgetStaticPtr itsPrompt;
	WidgetButtonPtr itsOkButton;
	WidgetButtonPtr itsCancelButton;

	SmartUtil::tstring itsPromptTxt;
	SmartUtil::tstring itsResultTxt;

	// Needed to allow the Widgets to be subclassed before the dialog appears,
	// but after the dialog is created.
	bool initDialog()
	{
		itsOkButton = subclassButton( IDOK );
		itsCancelButton = subclassButton( IDCANCEL );
		itsPrompt = subclassStatic( IDC_PROMPT );
		itsTextBox = subclassTextBox( IDC_NUMEDIT );

		itsOkButton->onClicked( & NumDialog::ok_clicked );
		itsCancelButton->onClicked( & NumDialog::cancel_clicked );
		onClosing( & NumDialog::closing );

		itsTextBox->setText( itsResultTxt );
		itsPrompt->setText( itsPromptTxt );

		// Here we EXPLICITLY sets the focus to our textbox Widget and selects ALL the text within it!
		// We could let windows set the focus to the control "it wishes" to, but since we're "control phreaks"... ;)
		itsTextBox->setSelection();
		itsTextBox->setFocus();

		// If we were to return false from this function we would instruct windows to set focus to the
		// first control in the dialog box that is visible, not disabled, and that has the WS_TABSTOP style.
		// But since WE return false we indicate that the system should NOT take care of setting the focus since WE have
		// done it already...!!
		return false;
	}

	// This one kicks in ONLY if user presses the cross in the upper right corner...
	// Behaviour then is to "cancel" the dialog...
	bool closing()
	{
		itsResultTxt = itsTextBox->getText();

		// Here we EXPLICITLY ends the dialog INSTEAD of returning true here!!
		// This means that this dialog would NOT be destroyed if it wasn't for us calling the DestroyDialog!!
		// If we were to return true from this function the result of the integer return value would NOT be the one
		// we expected it to be!!  ( we're expecting a "cancel" event)
		// Note!
		// This allows to make "pressing the cross up right" in the dialog mean "OK" instead cancel!!
		this->endDialog( IDCANCEL ); // IDOK or IDCANCEL or any other value.

		// Do NOT destroy the window... we rather want the "EndDialog" message to be dispatched...
		return false;
	}

	void ok_clicked( WidgetButtonPtr btn )
	{
		itsResultTxt = itsTextBox->getText();
		endDialog( IDOK ); // IDOK or IDCANCEL or any other value.
	}

	void cancel_clicked( WidgetButtonPtr btn )
	{
		endDialog( IDCANCEL ); // IDOK or IDCANCEL or any other value.
	}
};

//-----------------------------------------------------------

/*      PURE modal dialog does without resource files.

USAGE:
		NumDialog nd( pointerToWidgetWhichShouldBeParent );
		nd.SetPrompt( _T("Enter your name:"), _T("john modal dialog")  );

		INT_PTR res= nd.createDialog(); // Blocks until the dialog is finished.
*/
class PureNumDialog
	: public WidgetFactory< WidgetModalDialog, PureNumDialog, MessageMapPolicyModalDialogWidget >
{
public:
	void SetPrompt( SmartUtil::tstring inPromptTxt, SmartUtil::tstring initialTxt )
	{
		itsPromptTxt = inPromptTxt;
		itsResultTxt = initialTxt;
	}

	SmartUtil::tstring & GetResponse()
	{
		return itsResultTxt;
	}

	PureNumDialog( Widget * parent )
		: Widget( parent )
	{
		onInitDialog( & PureNumDialog::initDialog ); // Mandatory

		DLGTEMPLATE DlgTemplate;
		DlgTemplate.style = DS_MODALFRAME | DS_FIXEDSYS | WS_POPUP | WS_CAPTION | WS_SYSMENU | DS_CENTER;
		DlgTemplate.dwExtendedStyle = 0;
		DlgTemplate.cdit = 0; // No dialog items in the dialog
		DlgTemplate.cx = 200; DlgTemplate.cy = 100;
		DlgTemplate.x = 0; DlgTemplate.y = 0;
		setDlgTemplate( DlgTemplate ); // Optionally override the default dialog template
	}

	~PureNumDialog()
	{
	}

private:
	WidgetTextBoxPtr itsTextBox;
	WidgetStaticPtr itsPrompt;
	WidgetButtonPtr itsOkButton;
	WidgetButtonPtr itsCancelButton;
	WidgetButtonPtr itsAnotherModalButton;

	SmartUtil::tstring itsPromptTxt;
	SmartUtil::tstring itsResultTxt;

	// Needed to allow the Widgets to be subclassed before the dialog appears,
	// but after the dialog is created.
	bool initDialog()
	{
		setText( _T( "A modal dialog without use of resource files" ) );
		onClosing( & PureNumDialog::closing );

		itsCancelButton = createButton();
		itsCancelButton->setText( _T( "Cancel" ) );
		itsCancelButton->onClicked( & PureNumDialog::cancel_clicked );

#ifdef BUG
		itsAnotherModalButton= createButton();
		itsAnotherModalButton->setText( _T( "Causes crash" ) );
		itsAnotherModalButton->onClicked( & PureNumDialog::modalFromModalDialog );
#endif

		itsOkButton = createButton();
		itsOkButton->setText( _T( "OK" ) );
		itsOkButton->onClicked( & PureNumDialog::ok_clicked );

		itsPrompt = createStatic();
		itsPrompt->setText( itsPromptTxt );

		itsTextBox = createTextBox();
		itsTextBox->setText( itsResultTxt );
		itsTextBox->setFocus();

		// A layout approach to positioning the controls.
		//      itsPrompt   textbox
		//
		//      cancel       ok
		//
		long border = 10;
		SmartWin::Rectangle dialogRect( getClientAreaSize() );
		SmartWin::Rectangle inputRect( dialogRect.row( 0, 4 ).shrink( border ) );
		SmartWin::Rectangle buttonsRect( dialogRect.row( 3, 4 ).shrink( border ) );

		int cols = 2;
		itsPrompt->setSizeAsRow( inputRect, cols, 0, border );
		itsTextBox->setSizeAsRow( inputRect, cols, 1, border );

		cols = 3;
		itsCancelButton->setSizeAsRow( buttonsRect, cols, 0, border );
		itsAnotherModalButton->setSizeAsRow( buttonsRect, cols, 1, border );
		itsOkButton->setSizeAsRow( buttonsRect, cols, 2, border );
		return false;
	}

	void ok_clicked( WidgetButtonPtr btn )
	{
		itsResultTxt = itsTextBox->getText();
		endDialog( IDOK ); // IDOK or IDCANCEL or any other value.
	}

	void cancel_clicked( WidgetButtonPtr btn )
	{
		endDialog( IDCANCEL ); // IDOK or IDCANCEL or any other value.
	}


	void modalFromModalDialog( WidgetButtonPtr btn )
	{
#ifdef BUG
		int int1 = - 8;
		float float1 = ( float ) 98.6;
		SmartUtil::tstring str1 = "hello";

		InDialog myInDialog( this, "First InDialog Attempt !" );
		myInDialog
			 .add( _T( "Temperature:" ), & int1 )
			 .add( _T( "Average temperature:" ), & float1 )
			 .add( _T( "Say:" ), & str1 )
			 .createDialog();
#endif
	}




	// This one kicks in ONLY if user presses the cross in the upper right corner...
	// Behaviour then is to "cancel" the dialog...

	bool closing()
	{
		itsResultTxt = itsTextBox->getText();

		// Here we EXPLICITLY ends the dialog INSTEAD of returning true here!!
		// This means that this dialog would NOT be destroyed if it wasn't for us calling the DestroyDialog!!
		// If we were to return true from this function the result of the integer return value would NOT be the one
		// we expected it to be!!  ( we're expecting a "cancel" event)
		// Note!
		// This gives us flexibilty in that we can also have the "pressing the cross up right" in the dialog to
		// mean "OK" instead cancel!!
		this->endDialog( IDCANCEL );

		// Do NOT destroy the window... we rather want the "EndDialog" message to be dispatched...
		return false;
	}
};

#endif
