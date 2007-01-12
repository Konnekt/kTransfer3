// $Revision: 1.6 $
#ifndef CentralWidget_h
#define CentralWidget_h

#include "SmartWin.h"

class ABMainWindow;
class ABTabWidget;

class ABCentralWidget : public SmartWin::WidgetFactory< SmartWin::WidgetChildWindow, ABCentralWidget >
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
	void sized( const SmartWin::WidgetSizedEventResult & sz );

	// Data members
	WidgetDataGridPtr listView;
	ABTabWidget * tabWidget;
};

#endif
