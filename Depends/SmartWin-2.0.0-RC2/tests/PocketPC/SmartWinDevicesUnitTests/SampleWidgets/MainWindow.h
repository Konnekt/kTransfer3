// $Revision: 1.5 $
/* Copyright (c) 2005 - Thomas Hansen
 * Part of the SmartWin++ library example code
 * Permission to use under the same terms as the actual library
 * The license to the library can be found at http://smartwinlib.org
 * License for library is Open Source and BSD
 */
#ifndef MainWindow_H
#define MainWindow_H

#include "SmartWin.h"

class TabSheetWidget;

class MainWindow
			: public sw::WidgetFactory< sw::WidgetWindow, MainWindow >
{
public:
	void initAndCreate();
	MainWindow();

private:
	TabSheetWidget * itsTabSheet;

	// Event handlers
	void sized( const sw::WidgetSizedEventResult & newSize );
};

#endif
