// $Revision: 1.9 $
/* Copyright (c) 2005 - Thomas Hansen
 * Part of the SmartWin++ library example code
 * Permission to use under the same terms as the actual library
 * The license to the library can be found at http://smartwinlib.org
 * License for library is Open Source and BSD
 */
#ifndef TabSheetWidget_H
#define TabSheetWidget_H

#include "SmartWin.h"

class MainWindow;
class DataGrid;
class Buttons;
class Sliders;
class Misc;

class TabSheetWidget
			: public sw::WidgetFactory< sw::WidgetTabSheet, TabSheetWidget >
{
private:
	DataGrid * itsDataGrid;
	Buttons * itsButtons;
	Sliders * itsSliders;
	Misc * itsMisc;

	// Event handlers
	void selectedChanged( WidgetTabSheetPtr This );

public:
	// For easy access, DON'T do this at home kids!!!!    ;)
	MainWindow * itsParent;
	void initAndCreate();
	void layout();
	TabSheetWidget( MainWindow * parent );
};

#endif
