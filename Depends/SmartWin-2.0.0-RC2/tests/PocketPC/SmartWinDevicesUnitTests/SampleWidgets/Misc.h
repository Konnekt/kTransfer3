// $Revision: 1.8 $
/* Copyright (c) 2005 - Thomas Hansen
 * Part of the SmartWin++ library example code
 * Permission to use under the same terms as the actual library
 * The license to the library can be found at http://smartwinlib.org
 * License for library is Open Source and BSD
 */
#ifndef Misc_H
#define Misc_H

#include "SmartWin.h"

class TabSheetWidget;

class Misc
			: public sw::WidgetFactory< sw::WidgetChildWindow, Misc >
{
private:
	// Data Members
	boost::shared_ptr< sw::Brush > itsBgBrushComboBox;
	COLORREF itsColorComboBox;

	// Widgets
	TabSheetWidget * itsParent;
	WidgetDateTimePickerPtr itsDate;
	WidgetTreeViewPtr itsTree;
	WidgetStaticPtr itsStatic;
	WidgetStaticPtr itsStatic2;
	WidgetComboBoxPtr itsCombo;

	// Event handlers
	void treeClicked( WidgetTreeViewPtr tree );
	void treeDblClicked( WidgetTreeViewPtr tree );
	void treeRightClicked( WidgetTreeViewPtr tree );
	void selectionChanged( WidgetTreeViewPtr tree );
	void comboClicked( WidgetComboBoxPtr cboBox );
	void comboFocused( WidgetComboBoxPtr cboBox );
	void comboNotFocused( WidgetComboBoxPtr cboBox );
	bool comboKeyPressed( WidgetComboBoxPtr cboBox, int key );
	void comboMouseMoved( WidgetComboBoxPtr cboBox, const sw::MouseEventResult & mouse );
	boost::shared_ptr< sw::Brush > backgroundColor( WidgetComboBoxPtr box, sw::Canvas & canvas );

public:
	// Other functions
	void layout();

	// Init functions and constructor
	void initAndCreate();
	Misc( TabSheetWidget * parent );
};

#endif
