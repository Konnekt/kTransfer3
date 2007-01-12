// $Revision: 1.13 $
/* Copyright (c) 2005 - Thomas Hansen
 * Part of the SmartWin++ library example code
 * Permission to use under the same terms as the actual library
 * The license to the library can be found at http://smartwinlib.org
 * License for library is Open Source and BSD
 */
#ifndef DataGrid_H
#define DataGrid_H

#include "SmartWin.h"

class TabSheetWidget;

class DataGrid
			: public sw::WidgetFactory< sw::WidgetChildWindow, DataGrid >
{
private:
	// Widgets
	TabSheetWidget * itsParent;
	WidgetDataGridPtr itsDataGrid;
	WidgetTextBoxPtr itsName, itsEmail, itsNick;
	WidgetStaticPtr itsStatName, itsStatEmail, itsStatNick;
	WidgetButtonPtr itsAddButton;
	WidgetStaticPtr itsInfo;
	WidgetMenuPtr itsMainMenu;
	WidgetMenuPtr itsCtxMenu;

	// Brush used in static widgets
	boost::shared_ptr< sw::Brush > itsBgBrush;

	// Brush used in DataGrid background color
	boost::shared_ptr< sw::Brush > itsBitmapBrush;

	// Currently "clicked column"
	int itsClickedColumn;

	// true if reversed sort is to occur
	bool itsShouldSortReverse;

	// Event handlers
	void clicked( WidgetButtonPtr btn );
	void getFocus( WidgetTextBoxPtr txt );
	void looseFocus( WidgetTextBoxPtr txt );
	boost::shared_ptr< sw::Brush > backgroundColor( WidgetStaticPtr stat, sw::Canvas & canvas );
	void columnClicked( WidgetDataGridPtr list, int colNo );
	int sortCompare( WidgetDataGridPtr list, LPARAM left, LPARAM right );
	void customDraw( WidgetDataGridPtr list, unsigned row, unsigned col, bool selected, sw::Canvas & canvas, const sw::Rectangle & rc );
	void selectionChanged( WidgetDataGridPtr list );
	void listClicked( WidgetDataGridPtr list );
	void listDblClicked( WidgetDataGridPtr list );
	void listRightClicked( WidgetDataGridPtr list );
	void menuEventHandler( WidgetMenuPtr menu, unsigned item );

public:
	// Other functions
	void layout();

	// Init functions and constructor
	void initAndCreate();
	DataGrid( TabSheetWidget * parent );
};

#endif
