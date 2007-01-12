// $Revision: 1.8 $
/* Copyright (c) 2005 - Thomas Hansen
 * Part of the SmartWin++ library example code
 * Permission to use under the same terms as the actual library
 * The license to the library can be found at http://smartwinlib.org
 * License for library is Open Source and BSD
 */
#ifndef Buttons_H
#define Buttons_H

#include "SmartWin.h"

class TabSheetWidget;

class Buttons
			: public sw::WidgetFactory< sw::WidgetChildWindow, Buttons >
{
private:
	// Widgets
	TabSheetWidget * itsParent;
	WidgetButtonPtr itsButton;
	WidgetButtonPtr itsImgButton;
	WidgetStaticPtr itsStatic;
	WidgetCheckBoxPtr itsCheckBox;
	boost::shared_ptr< sw::Bitmap > itsBgBitmap;

	// Event handlers
	void clicked( WidgetButtonPtr btn );
	void dblClicked( WidgetButtonPtr btn );
	void paintBitmapButton( WidgetButtonPtr btn, sw::Canvas & c );

public:
	// Other functions
	void layout();

	// Init functions and constructor
	void initAndCreate();
	Buttons( TabSheetWidget * parent );
};

#endif
