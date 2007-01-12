// $Revision: 1.7 $
/* Copyright (c) 2005 - Thomas Hansen
 * Part of the SmartWin++ library example code
 * Permission to use under the same terms as the actual library
 * The license to the library can be found at http://smartwinlib.org
 * License for library is Open Source and BSD
 */
#ifndef Sliders_H
#define Sliders_H

#include "SmartWin.h"

class TabSheetWidget;

class Sliders
			: public sw::WidgetFactory< sw::WidgetChildWindow, Sliders >
{
private:
	// Data members
	boost::shared_ptr< sw::Brush > itsBgBrush;

	// Widgets
	TabSheetWidget * itsParent;
	WidgetSpinnerPtr itsSpinner;
	WidgetSliderPtr itsSlider;
	WidgetProgressBarPtr itsProgress;
	WidgetTextBoxPtr itsSpinnerBuddy;
	WidgetTextBoxPtr itsSliderBuddy;

	// Event handlers
	void sliderMoved( WidgetSliderPtr slider );
	boost::shared_ptr< sw::Brush > backgroundColor( WidgetTextBoxPtr slider, sw::Canvas & canvas );

public:
	// Other functions
	void layout();

	// Used in global event handler, public for easiness
	sw::BitmapPtr itsBitmap;

	// Init functions and constructor
	void initAndCreate();
	Sliders( TabSheetWidget * parent );
};

#endif
