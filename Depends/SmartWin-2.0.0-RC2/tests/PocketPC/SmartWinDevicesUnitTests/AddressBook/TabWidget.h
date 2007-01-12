// $Revision: 1.6 $
#ifndef TabWidget_h
#define TabWidget_h
#include "SmartWin.h"

class ABCentralWidget;

// Mark we COULD have made things completely like it is in the QT Address Book sample and NOT having this TabSheet being
// it's own class but then we wouldn't have done it like it is "supposed" to be in SmartWin++ and it wouldn't have
// used SmartWin++ in an "optimal" way...
class ABTabWidget : public SmartWin::WidgetFactory< SmartWin::WidgetTabSheet, ABTabWidget >
{
public:
	ABTabWidget( ABCentralWidget * parent );

private:
	// Init and helper functions
	void createChildWidgets();
	void layout();
	WidgetStatic * createStaticHelper( WidgetChildWindow * parent, const SmartUtil::tstring & txt );
	WidgetTextBox * createTextBoxHelper( WidgetChildWindow * parent, WidgetTextBox::Seed & cs );
	WidgetCheckBox * createCheckBoxHelper( WidgetChildWindow * parent, WidgetCheckBox::Seed & cs, const SmartUtil::tstring & txt );

	// Event handlers
	void selectedChanged( WidgetTabSheetPtr This );
	void sized( WidgetTabSheetPtr This, const SmartWin::WidgetSizedEventResult & rc );
	void addFunc( WidgetButtonPtr btn );
	void searchFunc( WidgetButtonPtr btn );

	// Widgets
	WidgetChildWindowPtr addChange;
	WidgetChildWindowPtr search;

	// Add New Item labels
	WidgetStaticPtr firstName, lastName, address, email, explanation;

	// Add button
	WidgetButtonPtr btnAdd;

	// Add new item text boxes
	WidgetTextBoxPtr txtFirstName, txtLastName, txtAddress, txtEmail;

	// Search text boxes
	WidgetTextBoxPtr txtSearchFirstName, txtSearchLastName, txtSearchAddress, txtSearchEmail;

	// Checkboxes for search for item
	WidgetCheckBoxPtr chkFirstName, chkLastName, chkAddress, chkEmail;

	// Search explenation
	WidgetStaticPtr searchExplenation;

	// Search button
	WidgetButtonPtr btnSearch;
};

#endif
