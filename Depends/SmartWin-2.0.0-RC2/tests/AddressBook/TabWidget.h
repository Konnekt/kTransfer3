// $Revision: 1.7 $
/// \file
///
/* Copyright (c) 2005 - Thomas Hansen
 * Part of the SmartWin++ library example code
 * Permission to use under the same terms as the actual library
 * The license to the library can be found at http://smartwinlib.org
 * License for library is Open Source and BSD
 */
#ifndef TabWidget_h
#define TabWidget_h

#include "SmartWin.h"

class ABCentralWidget;

// Note we COULD have made things completely like it is in the QT Address Book
// sample and NOT having this TabSheet being it's own class but then we wouldn't
// have done it like it is "supposed" to be in SmartWin++ and it wouldn't have used
// SmartWin++ in an "optimal" way...
class ABTabWidget : public sw::WidgetFactory< sw::WidgetTabSheet, ABTabWidget >
{
public:
	ABTabWidget( ABCentralWidget * parent );

private:
	// Init and helper functions
	void createChildWidgets();
	void layout();
	WidgetStaticPtr createStaticHelper( WidgetChildWindowPtr parent, sw::FontPtr & font, const SmartUtil::tstring & txt );
	WidgetTextBoxPtr createTextBoxHelper( WidgetChildWindowPtr parent, sw::FontPtr & font, WidgetTextBox::Seed & cs );
	WidgetCheckBoxPtr createCheckBoxHelper( WidgetChildWindowPtr parent, sw::FontPtr & font, WidgetCheckBox::Seed & cs, const SmartUtil::tstring & txt );

	// Event handlers
	void selectedChanged( WidgetTabSheetPtr This );
	void sized( WidgetTabSheetPtr This, const sw::WidgetSizedEventResult & rc );
	void addFunc( WidgetButtonPtr btn );
	void searchFunc( WidgetButtonPtr btn );

	// Widgets
	WidgetChildWindowPtr itsAddChange;
	WidgetChildWindowPtr itsSearch;

	// Add New Item labels
	WidgetStaticPtr itsFirstName, itsLastName, itsAddress, itsEmail, itsExplanation;

	// Add button
	WidgetButtonPtr itsBtnAdd;

	// Add new item text boxes
	WidgetTextBoxPtr itsTxtFirstName, itsTxtLastName, itsTxtAddress, itsTxtEmail;

	// Search text boxes
	WidgetTextBoxPtr itsTxtSearchFirstName, itsTxtSearchLastName, itsTxtSearchAddress, itsTxtSearchEmail;

	// Checkboxes for search for item
	WidgetCheckBoxPtr itsChkFirstName, itsChkLastName, itsChkAddress, itsChkEmail;

	// Search explanation
	WidgetStaticPtr itsSearchExplanation;

	// Search button
	WidgetButtonPtr itsBtnSearch;
};

#endif
