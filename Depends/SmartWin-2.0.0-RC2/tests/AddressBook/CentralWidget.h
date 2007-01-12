// $Revision: 1.9 $
/* Copyright (c) 2005 - Thomas Hansen
 * Part of the SmartWin++ library example code
 * Permission to use under the same terms as the actual library
 * The license to the library can be found at http://smartwinlib.org
 * License for library is Open Source and BSD
 */
#ifndef CentralWidget_h
#define CentralWidget_h

#include "SmartWin.h"

class ABMainWindow;
class ABTabWidget;

class ABCentralWidget : public sw::WidgetFactory< sw::WidgetChildWindow, ABCentralWidget >
{
public:
	ABCentralWidget( ABMainWindow * parent );

	// BIG difference!
	// While Qt uses it's own custom classes for e.g. strings etc, SmartWin++ uses the built in classes where possible
	// like for instance the std::string instead of a custom string like QString!
	void save( const SmartUtil::tstring & path );
	void load( const SmartUtil::tstring & path );
	void add( const SmartUtil::tstring & firstName, const SmartUtil::tstring & lastName,
				  const SmartUtil::tstring & address, const SmartUtil::tstring & email );
	void search( const SmartUtil::tstring & firstName, const SmartUtil::tstring & lastName,
				 const SmartUtil::tstring & address, const SmartUtil::tstring & email );

private:
	void setupTabWidget();
	void setupListView();

	// Event handlers
	void sized( const sw::WidgetSizedEventResult & sz );

	// Data members
	WidgetDataGridPtr itsListView;
	ABTabWidget * itsTabWidget;
};

#endif
