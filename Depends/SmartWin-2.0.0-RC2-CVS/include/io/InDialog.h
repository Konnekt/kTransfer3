// $Revision: 1.13 $
/*
  Copyright (c) 2005, Thomas Hansen
  All rights reserved.

  Redistribution and use in source and binary forms, with or without modification,
  are permitted provided that the following conditions are met:

	  * Redistributions of source code must retain the above copyright notice,
		this list of conditions and the following disclaimer.
	  * Redistributions in binary form must reproduce the above copyright notice,
		this list of conditions and the following disclaimer in the documentation
		and/or other materials provided with the distribution.
	  * Neither the name of the SmartWin++ nor the names of its contributors
		may be used to endorse or promote products derived from this software
		without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
  INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#ifndef InDialog_87538_H
#define InDialog_87538_H

#include "SmartWin.h"
using namespace SmartWin;
using namespace std;

#include "boost/any.hpp"
#include "InDialogClasses.h"

namespace SmartWin
{

/// Provides a general purpose input modal dialog for numbers and strings.
/** An InDialog can accept pointers to a variety of types, and build a input dialog
 * for those variables.  There can be one or many variables.
 * The pupose of this dialog is to satisfy the needs of many possible dialogs
 * with a general purpose dialog.
 *
 * First you use the constructor to set the parent window (this), the Dialog
 * title., and optionally a font. <br>
 * Then you call the templated  add( const SmartUtil::tstring & prompt, TypeIn val )
 * function to pass the address of your variables. <br>
 * Finally, you call createDialog() to start the dialog running, and receive either
 * IDCANCEL or IDOK. The variables are automatically set to the values input by the
 * user after the OK button is pressed.
 *
USAGE: <br>

<pre>
	int int1= -8;
	BoundedLong     bl( 1994, 1900, 2050 );
	float   float1= (float)98.6;
	double  doub1= 876.543;
	SmartUtil::tstring  str1= "hello";
	PasswordString pass1;
	bool        bool1= true;
	COLORREF    background= RGB( 20, 40, 80 );

	ChoiceString    cs( _T("scissors|rock|paper"), 2 );

	FileString      loadFs( "C:\\path\\gumbo.txt" );
	loadFs.addFilter( "JPG files", "*.jpg" );

	FileString      saveFs( "", false );
	saveFs.addFilter( "JPG files", "*.jpg" );

	InDialog    myInDialog( this, "First InDialog Attempt !" );
	if ( IDCANCEL == myInDialog
	.add(         _T("Input file ..."), &loadFs )
	.add(        _T("Output file ..."), &saveFs )
	.add(           _T("Temperature:"), &int1 )
	.add(   _T("Average temperature:"), &float1 )
	.add(        _T("Average weight:"), &doub1 )
	.add( _T("Birth year (1900-2050)"), &bl )
	.add(                _T("Fries ?"), &bool1 )
	.add(                   _T("Say:"), &str1 )
	.add(              _T("Password:"), &pass1 )
	.add(                 _T("Throw:"), &cs )
	.createDialog() ) return;
</pre>
*   The resulting dialog:
*   - is sized to show each prompt and variable initial value in two columns.
*   - prevents illegal values from being entered.
*   - has an ok button that sets variables when pressed, and returns IDOK.
*   - has a cancel button which leaves the variables untouched, and returns IDCANCEL
*   - can optionally take a font in the constructor and size the fields accordingly.
*   - has tab control between input fields.
*
*/

class InDialog
	: public WidgetFactory< WidgetModalDialog, InDialog, MessageMapPolicyModalDialogWidget >
{
public:

	/// Constructor InDialog( this, "Title" ) uses a default font.
	InDialog( Widget * parent, SmartUtil::tstring inTitle )
		: Widget( parent ),
		  itsTitle( inTitle )
	{
		onInitDialog( & InDialog::initDialog ); // Mandatory
		setFont = false;
	}

	/// Constructor that allows selection of a Font for the dialog.
	InDialog( Widget * parent, SmartUtil::tstring inTitle, FontPtr inFont )
		: Widget( parent ),
		  itsTitle( inTitle ),
		  itsFont( inFont )
	{
		onInitDialog( & InDialog::initDialog ); // Mandatory
		setFont = true;
	}

	/// prepare the dialog with same title to be used again.
	void clear()
	{
		itsPrompts.clear();
		itsVariables.clear();
		itsIndicies.clear();

		itsStatics.clear();
		itsBoxes.clear();
		itsChecks.clear();
		itsCombos.clear();
		itsPrompts.clear();
		itsColors.clear();

		maxLabel = maxInputArea = Point( 0, 0 );
	}

	/// Add a prompt and address of a variable of a variety of types.
	/** The core possible types are: <br>
	* bool *, <br>
	* int *, float *, double *, <br>
	* SmartUtil::tstring * <br>
	* SYSTEMTIME *   (Via a Calendar dialog)<br>
	* COLORREF *	(Via a color chooser dialog);
	* The specially defined types (See InDialogClasses.h) are: <br>
	*  PasswordString *, FileString *, DirString *, BoundedLong *, ChoiceString *
	*
	* The type passed in is the template TypeIn. 
	*
	* The initial contents are displayed as a default,
	* and the input result is returned in the variable if the dialog is commited.
	**/
	template< typename TypeIn >
	InDialog & add( const SmartUtil::tstring & prompt, TypeIn val )
	{
		itsVariables.push_back( val );
		itsPrompts.push_back( prompt );
		// We simply store the parameters until the dialog has been initialized
		// since the widgets can be created until then.

		setDialogTemplate(); // Done each time so it will be current for number and type of variables.
		return * this;
	}

private:

	// MEMBER FUNCTIONS

	// Override the default Modal Dialog parameters.
	void setDialogTemplate()
	{
		DLGTEMPLATE DlgTemplate;
		DlgTemplate.style = DS_MODALFRAME | WS_CAPTION | WS_SYSMENU | WS_SIZEBOX | WS_POPUP;
		DlgTemplate.dwExtendedStyle = 0; // WS_EX_CONTROLPARENT;

		DlgTemplate.cdit = 0; // No dialog items in the dialog
		DlgTemplate.x = 0; DlgTemplate.y = 0;	// Positioning done now.
		DlgTemplate.cx = 0; DlgTemplate.cy = 0;	// Sizing done at dialog init time.

		setDlgTemplate( DlgTemplate ); // Optionally override the default dialog template
	}


	// utility routine to add a WidgetStatic and WidgetRichTextBox for a textual variable.
	// OUT:  Added an entry to itsStatics and itsBoxes
	//
	void addPromptAndText( const SmartUtil::tstring & prompt, const SmartUtil::tstring & initialVal, int extraStyle )
	{
			addPrompt( prompt );

			// Add textual value
			WidgetRichTextBox::Seed richTextBoxCS;

			richTextBoxCS.style = extraStyle | ES_LEFT | WS_VISIBLE | WS_BORDER | WS_TABSTOP;
			WidgetRichTextBox * wtb = createRichTextBox( richTextBoxCS );
			wtb->setText( initialVal );
			if ( setFont )
				wtb->setFont( itsFont );
			else
				wtb->setFont( SystemFont );

			Point textSize;
			if ( initialVal.size() > 4 )
			{
				textSize = wtb->getTextSize( initialVal ); // The actual size of the initial value.
			}
			else
			{
				textSize = wtb->getTextSize( _T("yT-^") ); // A min size with descenders and a high symbol.
			}
			maxInputArea.maxOf( textSize );
			// wtb->setBackgroundColor( RGB( rand()%255, rand()%255, rand()%255 ) );

			itsIndicies.push_back( ( int ) itsBoxes.size() );
			itsBoxes.push_back( wtb );
	}

	// utility routine to add a WidgetStatic.
	//
	void addPrompt( const SmartUtil::tstring & prompt )
	{
		// Label
		WidgetStatic::Seed staticCS;

		staticCS.style = WS_VISIBLE | ES_RIGHT | SS_NOTIFY;
		WidgetStatic * ws = createStatic( staticCS );
		ws->setText( prompt );
		if ( setFont )
			ws->setFont( itsFont );
		else
			ws->setFont( SystemFont );
		itsStatics.push_back( ws );
		maxLabel.maxOf( ws->getTextSize( prompt ) );
	}

	// Extract the initial string value and create the row for a variable.
	template< class numtype >
	void putNumericIntoString( int b )
	{
		SmartUtil::tstringstream ss;
		numtype * val = boost::any_cast < numtype * >( itsVariables.at( b ) );
		ss << * val;
		addPromptAndText( itsPrompts.at( b ), ss.str(), ES_NUMBER );
	}

	// Parse through all the variables and build each Dialog row
	bool initDialog()
	{
		setText( itsTitle );

		itsStatics.clear(); itsBoxes.clear();
		itsExtraAroundText = Point( 16, 12 ); // X,Y pixels added to calculated text height and width.
		itsBorder = Point( 4, 12 ); // Seperation between widgets


		// Prepare the widgets for use.
		for ( unsigned int b = 0; b < itsVariables.size(); b++ )
		{
			// long *
			if ( itsVariables.at( b ).type() == typeid( long * ) )
			{
				putNumericIntoString< long >( b );
				continue;
			}

			// int *
			if ( itsVariables.at( b ).type() == typeid( int * ) )
			{
				putNumericIntoString< int >( b );
				continue;
			}

			// float *
			if ( itsVariables.at( b ).type() == typeid( float * ) )
			{
				putNumericIntoString< float >( b );
				continue;
			}

			// double *
			if ( itsVariables.at( b ).type() == typeid( double * ) )
			{
				putNumericIntoString< double >( b );
				continue;
			}

			// class BoundedLong *
			if ( itsVariables.at( b ).type() == typeid( class BoundedLong * ) )
			{
				SmartUtil::tstringstream ss;
				BoundedLong * valptr = boost::any_cast < BoundedLong * >( itsVariables.at( b ) );
				ss << valptr->itsVal;
				addPromptAndText( itsPrompts.at( b ), ss.str(), 0 );
				continue;
			}

			// SmartUtil::tstring *
			if ( itsVariables.at( b ).type() == typeid( SmartUtil::tstring * ) )
			{
				SmartUtil::tstring * val = boost::any_cast < SmartUtil::tstring * >( itsVariables.at( b ) );
				addPromptAndText( itsPrompts.at( b ), * val, 0 );
				continue;
			}

			// PasswordString *
			if ( itsVariables.at( b ).type() == typeid( PasswordString * ) )
			{
				PasswordString * valptr = boost::any_cast < PasswordString * >( itsVariables.at( b ) );
				addPromptAndText( itsPrompts.at( b ), valptr->itsString, ES_PASSWORD );
				continue;
			}

			// FileString *
			if ( itsVariables.at( b ).type() == typeid( FileString * ) )
			{
				FileString * val = boost::any_cast < FileString * >( itsVariables.at( b ) );
				addPromptAndText( itsPrompts.at( b ), val->itsFilePath, 0 );
				itsStatics.at( b )->onClicked( & InDialog::FetchFilename );
				continue;
			}


			// DirString *
			if ( itsVariables.at( b ).type() == typeid( DirString * ) )
			{
				addPrompt( itsPrompts.at( b ) );
				DirString * valptr = boost::any_cast < DirString * >( itsVariables.at( b ) );
							
				vector< SmartUtil::tstring > choices = valptr->genNearDirs();

				// Put all the strings into the WidgetComboBox
				// We calculate that the Combo needs enough space for 2 lines at least.
				WidgetComboBox::Seed seed= 	WidgetComboBox::getDefaultSeed();
				if ( valptr->allowsCreation() ) {
					seed.style= WS_CHILD | WS_VISIBLE | CBS_DROPDOWN | CBS_HASSTRINGS | WS_VSCROLL;
				}
				WidgetComboBox * combo = createComboBox(seed);
				combo->addRemoveStyle( WS_TABSTOP, true );
				if ( setFont )
					combo->setFont( itsFont );
				else
					combo->setFont( SystemFont );

				int numChoices = ( int ) choices.size();
				for ( int i = 0; i < numChoices; i++ )
				{
					combo->addValue( choices[ i ] );
					Point OptionSize = combo->getTextSize( choices[ i ] );
					OptionSize.x += 24; // Account for \/ at right of combo box.
					maxInputArea.maxOf( OptionSize );
				}
				combo->setSelectedIndex( 0 );
				combo->onSelectionChanged( & InDialog::dirSelectionChanged );

				itsIndicies.push_back( ( int ) itsCombos.size() );
				itsCombos.push_back( combo );
				continue;
			}



			// ChoiceString *
			if ( itsVariables.at( b ).type() == typeid( ChoiceString * ) )
			{
				addPrompt( itsPrompts.at( b ) );
				ChoiceString * valptr = boost::any_cast < ChoiceString * >( itsVariables.at( b ) );

				// Put all the strings into the WidgetComboBox
				// We calculate that the Combo needs enough space for 2 lines at least.
				WidgetComboBox * combo = createComboBox();
				combo->addRemoveStyle( WS_TABSTOP, true );
				if ( setFont )
					combo->setFont( itsFont );
				else
					combo->setFont( SystemFont );

				int numChoices = ( int ) valptr->itsPossibleStrings.size();
				for ( int i = 0; i < numChoices; i++ )
				{
					combo->addValue( valptr->itsPossibleStrings[ i ] );
					Point OptionSize = combo->getTextSize( valptr->itsPossibleStrings[ i ] );
					OptionSize.x += 24; // Account for \/ at right of combo box.
					maxInputArea.maxOf( OptionSize );
				}
				combo->setSelectedIndex( valptr->itsChoosenIndex );

				itsIndicies.push_back( ( int ) itsCombos.size() );
				itsCombos.push_back( combo );
				continue;
			}

			// bool *
			if ( itsVariables.at( b ).type() == typeid( bool * ) )
			{
				addPrompt( itsPrompts.at( b ) );

				bool * val = boost::any_cast < bool * >( itsVariables.at( b ) );
				WidgetCheckBox * check = createCheckBox();
				check->setChecked( * val );
				check->addRemoveStyle( BS_TOP | WS_TABSTOP, true ); // BS_LEFTTEXT BS_VCENTER |
				if ( setFont )
					check->setFont( itsFont );
				else
					check->setFont( SystemFont );

				itsIndicies.push_back( ( int ) itsChecks.size() );
				itsChecks.push_back( check );

				maxInputArea.maxOf( check->getTextSize( _T( "[ ]" ) ) );
				continue;
			}

			// SYSTEMTIME *
			if ( itsVariables.at( b ).type() == typeid( SYSTEMTIME * ) )
			{
				addPrompt( itsPrompts.at( b ) );
				SYSTEMTIME * st = boost::any_cast < SYSTEMTIME * >( itsVariables.at( b ) );

				WidgetDateTimePicker * date = createDateTimePicker();
				date->setDateTime( * st );
				if ( setFont )
				{
					date->setFont( itsFont );
				}
				else
				{
					date->setFont( SystemFont );
				}

				itsIndicies.push_back( ( int ) itsDates.size() );
				itsDates.push_back( date );
				maxInputArea.maxOf( itsStatics.at( b )->getTextSize( _T("12/12/1999") ) );
				continue;
			}

			// COLORREF *
			if ( itsVariables.at( b ).type() == typeid( COLORREF * ) )
			{
				// Put the starting color in itsColors
				addPromptAndText( itsPrompts.at( b ), _T(""), 0 );
				COLORREF * colorPtr = boost::any_cast < COLORREF * >( itsVariables.at( b ) );

				WidgetRichTextBox * wtb = itsBoxes[ itsIndicies[ b ] ];
				wtb->setBackgroundColor( * colorPtr );
				itsColors.insert( pair< int, COLORREF >( b, * colorPtr ) );
				itsStatics.at( b )->onClicked( & InDialog::chooseColor );
				continue;
			}
	
			// Catch failure to pass in a supported type
			// so that itsPrompts and itsVariables stay coherent.
			// Although type().name() is a mem leak, this is a fatal error anyhow.
			//
			SmartUtil::tstring msg( _T("InDialog unsupported type: ") );
			appendTo( msg, itsVariables.at( b ).type().name() );	
			addPromptAndText( itsPrompts.at( b ), msg, 0 );			
		}

		// Add the Cancel and OK buttons.
		itsCancelBut = createButton();
		itsCancelBut->setText( _T("cancel") );
		itsCancelBut->addRemoveStyle( WS_TABSTOP, true );
		itsCancelBut->onClicked( & InDialog::OnCancel );
		if ( setFont )
			itsCancelBut->setFont( itsFont );
		else
			itsCancelBut->setFont( SystemFont );

		itsOkBut = createButton();
		itsOkBut->setText( _T("ok") );
		itsOkBut->addRemoveStyle( WS_TABSTOP, true );
		itsOkBut->onClicked( & InDialog::OnOk );
		if ( setFont )
			itsOkBut->setFont( itsFont );
		else
			itsOkBut->setFont( SystemFont );

		onClosing( & InDialog::isClosing );
		onSized( & InDialog::isResized );

		setDialogBounds(); // Calculate a size for the dialog
		layout();

		if ( itsBoxes.size() > 0 ) itsBoxes[0]->setFocus();
		return false; // Indicate that the focus was set.
	}



	// Bring up another dialog to accept a file name by browsing the file system.
	// ( used by FileString variables )
	//
	void FetchFilename( WidgetStaticPtr prompt )
	{
		for ( unsigned b = 0; b < itsPrompts.size(); b++ )
		{
			if ( prompt == itsStatics[b] )
			{
				// Display a file dialog, and put the selected file in the textbox.
				FileString * valptr = boost::any_cast < FileString * >( itsVariables.at( b ) );

				SmartUtil::tstring filePath;
				if ( valptr->itsLoadFile )
				{
					WidgetLoadFile dlg = createLoadFile();
					for ( unsigned f = 0; f < valptr->itsFilters.size(); f++ )
					{
						dlg.addFilter( valptr->itsFilters[f].first, valptr->itsFilters[f].second );
					}
					dlg.activeFilter( valptr->itsFilterNo );
					filePath = dlg.showDialog();
				}
				else
				{
					WidgetSaveFile dlg = createSaveFile();
					for ( unsigned f = 0; f < valptr->itsFilters.size(); f++ )
					{
						dlg.addFilter( valptr->itsFilters[f].first, valptr->itsFilters[f].second );
					}
					dlg.activeFilter( valptr->itsFilterNo );
					filePath = dlg.showDialog();
				}

				// if ( valptr->itsRelative ) ChangeAbsToRelative( filePath );
				itsBoxes[ itsIndicies[b] ]->setText( filePath );
				return;
			}
		}
	}


	void dirSelectionChanged( WidgetComboBoxPtr comboBox )
	{
		// Need to find which DirString has this comboBox.
		for ( unsigned int b = 0; b < itsVariables.size(); b++ ) {
			if ( itsVariables.at( b ).type() == typeid( DirString * ) )	{
				if ( comboBox == itsCombos[ itsIndicies[b] ] ) {

					DirString * valptr = boost::any_cast < DirString * >( itsVariables.at( b ) );
				
					int seldex= comboBox->getSelectedIndex();
					valptr->setDir( comboBox->getValue( seldex ) );
					vector< SmartUtil::tstring > choices = valptr->genNearDirs();
					
					comboBox->removeAllItems();
					for ( unsigned int i = 0; i < choices.size(); i++ ) {
						comboBox->addValue( choices[ i ] );
					}
					comboBox->setSelectedIndex( 0 );	// 
					return;
				}
			}
		}
	}



	// Use the WidgetChooseColor to get a color,
	// Color the textbox, and store the color in itsColors
	void chooseColor( WidgetStaticPtr prompt )
	{
		WidgetChooseColor dlg = createChooseColor();
		WidgetChooseColor::ColorParams color = dlg.showDialog( true );
		if ( color.userPressedOk() )
		{
			for ( unsigned b = 0; b < itsPrompts.size(); b++ )
			{
				if ( prompt == itsStatics[b] )
				{
					WidgetRichTextBox * wtb = itsBoxes[ itsIndicies[ b ] ];
					wtb->setBackgroundColor( color.getColor() );
					map< int, COLORREF >::iterator it = itsColors.find( b );
					if ( it != itsColors.end() )
					{
						it->second = color.getColor();
					}
				}
			}
		}
	}

	void isResized( const WidgetSizedEventResult & sz )
	{
		layout(); // Use has choosen a size for the dialog
		updateWidget();
	}

	int rows_needed()
	{
		return( 1 + ( int ) itsVariables.size() );
	}

	// Establish the overall size of the dialog
	// based on the calculated size of each row.
	//
	void setDialogBounds()
	{
		Point titleSize = getTextSize( itsTitle );
		if ( itsTitle.size() < 4 )
		{
			titleSize = getTextSize( _T("yT-^") ); // A min size with descenders and a high symbol.
		}
		titleSize.x += 24;	// Space for close icon
		titleSize += itsExtraAroundText;
		maxLabel += itsExtraAroundText;
		maxInputArea += itsExtraAroundText;

		Point cellDim( maxInputArea );

		Point size;
		size.x = maxLabel.x + cellDim.x + ( 3 * itsBorder.x ); // bb ppp bb iii bb
		if ( titleSize.x > size.x ) size.x = titleSize.x;

		int rows = rows_needed();
		size.y = titleSize.y + ( cellDim.y * rows ) + ( itsBorder.y * ( 1 + rows ) );

		// Ensure that it fits on the screen.
		stringstream ss;
		Point deskSize = getDesktopSize();
		Point position = getScreenPosition();
		// Not larger than the screen
		if ( size.x > deskSize.x ) size.x = deskSize.x;	
		if ( size.y > deskSize.y ) size.y = deskSize.y;

		// Not positioned so it goes off the screen
		if ( position.x + size.x > deskSize.x ) position.x = deskSize.x - size.x;
		if ( position.y + size.y > deskSize.y ) position.y = deskSize.y - size.y;
		setBounds( position, size );
	}


	// pos_size holds the sizes assuming that each column should be 50%.
	// But we want to adjust this by the actual ratio of the maximum xs.
	//
	void adjustSizesByRatio( SmartWin::Rectangle & pos_size,
		SmartWin::Rectangle & pos_sizeLabel, SmartWin::Rectangle & pos_sizeInput )
	{
		float fract = maxLabel.x / (float)(maxLabel.x + maxInputArea.x);
		int rowWidth= 2 * pos_size.size.x;
		pos_sizeLabel= pos_size;
		pos_sizeLabel.size.x= (long)(rowWidth * fract);

		pos_sizeInput= pos_size;
		pos_sizeInput.size.x=  (long) (rowWidth * (1.0 - fract));
	}

	// Allocate space for each widget in the dialog
	// Given the size of the dialog, allocate each row
	//
	void layout()
	{
		if ( 0 == itsVariables.size() ) return;

		itsPlace.setBoundsBorders( getClientAreaSize(), itsBorder.x, itsBorder.y );

		// maxLabel.x
		SmartWin::Rectangle pos_size, pos_sizeLabel, pos_sizeInput;
		itsPlace.sizeOfCell( rows_needed(), 2, pos_size.size );
		adjustSizesByRatio( pos_size, pos_sizeLabel, pos_sizeInput );

		for ( unsigned int b = 0; b < itsVariables.size(); b++ )
		{
			itsPlace.positionToRight( pos_sizeLabel );
			itsStatics.at( b )->setBounds( pos_sizeLabel );

			itsPlace.positionToRight( pos_sizeInput );

			if ( itsVariables.at( b ).type() == typeid( bool * ) )
			{
				itsChecks.at( itsIndicies[b] )->setBounds( pos_sizeInput );
			}
			else

			if ( itsVariables.at( b ).type() == typeid( SYSTEMTIME * ) )
			{
				itsDates.at( itsIndicies[b] )->setBounds( pos_sizeInput );
			}
			else

			if ( ( itsVariables.at( b ).type() == typeid( ChoiceString * ) ) ||
				 ( itsVariables.at( b ).type() == typeid( DirString * ) )
				)
			{
				itsCombos[ itsIndicies[b] ]->setBounds( pos_sizeInput.pos,
				Point( pos_sizeInput.size.x, pos_sizeInput.size.y * 5 ) );
			}
			else
			{
				// All other types just have a text box.
				itsBoxes[ itsIndicies[b] ]->setBounds( pos_sizeInput );
			}
		}

		itsPlace.positionToRight( pos_sizeLabel );
		itsCancelBut->setBounds( pos_sizeLabel );

		itsPlace.positionToRight( pos_sizeInput );
		itsOkBut->setBounds( pos_sizeInput );
	}

	// Convert numeric strings to a number.
	// OUT: Return if the string had a valid numeric value
	// Since streams leave variables alone if they can't parse a number, we have to
	// check if the string happened to be the initial value.
	//
	template< class numtype >
	bool getNumericFromString( const SmartUtil::tstring & inStr, numtype & val )
	{
		SmartUtil::tstringstream ss( inStr );
		val = 1;
		ss >> val;
		if ( 1 != val ) return true; //

		val = 2;
		SmartUtil::tstringstream ss2( inStr );
		ss2 >> val;
		return( 2 != val );
	}

	// Pull the values from the textbox, complain if they are not numbers. and set
	// the variable if they are.
	//
	template< class numtype >
	bool validNumeric( int dex, const SmartUtil::tstring & reject )
	{
		SmartUtil::tstring str = itsBoxes[ itsIndicies[dex] ]->getText();
		numtype val;
		bool valid = getNumericFromString< numtype >( str, val );
		if ( valid )
		{
			numtype * valptr = boost::any_cast < numtype * >( itsVariables.at( dex ) );
			* valptr = val;
		}
		else
		{
			str += reject;
			createMessageBox().show( str, itsPrompts[dex] );
			itsBoxes[ itsIndicies[dex] ]->setFocus();
		}

		return valid;
	}


	void appendTo( SmartUtil::tstring & dest, const char * zeroTermTxt )
	{
		const char * beg= zeroTermTxt;
		const char * end= zeroTermTxt + strlen(zeroTermTxt);
		dest += SmartUtil::tstring( beg, end );
	}




	// The OK button has been pressed, so transfer all the values back into the
	// variables. But don't exit the dialog until all rows satisfy their
	// constraints.
	//
	void OnOk( WidgetButton * btn )
	{
		// Transfer the TextBox contents back to calling window.
//      for ( unsigned int b=0; b< itsBoxes.size(); b++ ) {
//          itsParams->at(b).val= itsBoxes.at(b)->getText();
//      }

		for ( unsigned int b = 0; b < itsVariables.size(); b++ )
		{
			// long *
			if ( itsVariables.at( b ).type() == typeid( long * ) )
			{
				if ( ! validNumeric< long >( b, _T( " is not a long" ) ) )
					return; // Don't exit.
			}

			// int *
			if ( itsVariables.at( b ).type() == typeid( int * ) )
			{
				if ( ! validNumeric< int >( b, _T( " is not an integer" ) ) )
					return; // Don't exit.
			}

			// float *
			if ( itsVariables.at( b ).type() == typeid( float * ) )
			{
				if ( ! validNumeric< float >( b, _T( " is not a float" ) ) )
					return; // Don't exit.
			}

			// double *
			if ( itsVariables.at( b ).type() == typeid( double * ) )
			{
				if ( ! validNumeric< double >( b, _T( " is not a double" ) ) )
					return; // Don't exit.
			}

			// class BoundedLong *
			if ( itsVariables.at( b ).type() == typeid( class BoundedLong * ) )
			{
				BoundedLong * valptr = boost::any_cast < BoundedLong * >( itsVariables.at( b ) );
				SmartUtil::tstring str = itsBoxes[ itsIndicies[b] ]->getText();
				long val;
				bool valid = getNumericFromString< long >( str, val );
				if ( valid && ( val <= valptr->itsMaxval ) && ( val >= valptr->itsMinval ) )
				{
						valptr->itsVal = val;
				}
				else
				{
					SmartUtil::tstringstream ss;
					ss << itsBoxes[ itsIndicies[b] ]->getText();
					ss << _T( " is not between " );
					ss << valptr->itsMinval << _T( " and " ) << valptr->itsMaxval;
					createMessageBox().show( ss.str(), itsPrompts[b] );
					itsBoxes[ itsIndicies[b] ]->setFocus();
					return;
				}
			}

			// SmartUtil::tstring *
			if ( itsVariables.at( b ).type() == typeid( SmartUtil::tstring * ) )
			{
				SmartUtil::tstring * valptr = boost::any_cast < SmartUtil::tstring * >( itsVariables.at( b ) );
				* valptr = itsBoxes[ itsIndicies[b] ]->getText();
			}

			// PasswordString *
			if ( itsVariables.at( b ).type() == typeid( PasswordString * ) )
			{
				SmartUtil::tstring str = itsBoxes[ itsIndicies[b] ]->getText();
				PasswordString * valptr = boost::any_cast < PasswordString * >( itsVariables.at( b ) );
				if ( 0 != valptr->itsKnownLength )
				{
					if ( str.size() != valptr->itsKnownLength )
					{
						SmartUtil::tstringstream ss; // Complain that the length was not correct.
						ss << _T( "Should be exactly " ) << valptr->itsKnownLength << " characters long.";
						createMessageBox().show( ss.str(), itsPrompts[b] );
						itsBoxes[ itsIndicies[b] ]->setFocus();
						return;
					}
				}
				valptr->itsString = str;
			}

			// FileString *
			if ( itsVariables.at( b ).type() == typeid( FileString * ) )
			{
				FileString * valptr = boost::any_cast < FileString * >( itsVariables.at( b ) );
				valptr->itsFilePath = itsBoxes[ itsIndicies[b] ]->getText();
			}

			// bool *
			if ( itsVariables.at( b ).type() == typeid( bool * ) )
			{
				bool * valptr = boost::any_cast < bool * >( itsVariables.at( b ) );
				* valptr = itsChecks[ itsIndicies[b] ]->getChecked();
			}

			// ChoiceString *
			if ( itsVariables.at( b ).type() == typeid( ChoiceString * ) )
			{
				ChoiceString * valptr = boost::any_cast < ChoiceString * >( itsVariables.at( b ) );
				valptr->itsChoosenIndex = itsCombos[ itsIndicies[b] ]->getSelectedIndex();
			}

			// DirString *
			if ( itsVariables.at( b ).type() == typeid( DirString * ) )
			{
				DirString * valptr = boost::any_cast < DirString * >( itsVariables.at( b ) );
				valptr->setDir( itsCombos[ itsIndicies[b] ]->getSelectedValue() );
			}

			// SYSTEMTIME
			if ( itsVariables.at( b ).type() == typeid( SYSTEMTIME * ) )
			{
				SYSTEMTIME * st = boost::any_cast < SYSTEMTIME * >( itsVariables.at( b ) );
				* st = itsDates[ itsIndicies[b] ]->getDateTime();
			}

			// COLORREF
			if ( itsVariables.at( b ).type() == typeid( COLORREF * ) )
			{
				COLORREF * colorPtr = boost::any_cast < COLORREF * >( itsVariables.at( b ) );
				map< int, COLORREF >::iterator it = itsColors.find( b );
				if ( it != itsColors.end() )
				{
					* colorPtr = it->second;
				}
			}
		}

		endDialog( IDOK ); // The dialog will close.
	}

	void OnCancel( WidgetButton * btn )
	{
		endDialog( IDCANCEL ); // IDOK or IDCANCEL or any other value.
	}

	// This one kicks in ONLY if user presses the cross in the upper right corner...
	// Behaviour then is to "cancel" the dialog...
	//
	bool isClosing()
	{
		this->endDialog( IDCANCEL );

		// Do NOT destroy the window... we rather want the "EndDialog" message to be dispatched...
		return false;
	}

	// MEMBER VARIABLES
	SmartUtil::tstring itsTitle;
	int itsX, itsY;
	FontPtr itsFont;
	bool setFont;

	vector< SmartUtil::tstring > itsPrompts; // Before InitDialog
	vector< boost::any > itsVariables; // Initial Value and final value
	vector< int > itsIndicies; // Index into widget per variable type.

	// Each variable type has a static in the left column and a known widget on the
	// right column.  itsIndicies maps the instance of the variable to a particular
	// widget in the vectors below.

	vector< WidgetStaticPtr > itsStatics; // After InitDialog
	vector < WidgetRichTextBox * > itsBoxes; // For numbers and strings
	vector < WidgetCheckBox * > itsChecks; // For boolean
	vector < WidgetComboBox * > itsCombos; // For ChoiceString and DirString
	vector < WidgetDateTimePicker * > itsDates; // For SYSTEMTIME
	map< int, COLORREF > itsColors;

	WidgetButton * itsOkBut, * itsCancelBut;

	Place itsPlace;
	Point maxLabel, maxInputArea;
	Point itsExtraAroundText, itsBorder;
};

}

#endif
