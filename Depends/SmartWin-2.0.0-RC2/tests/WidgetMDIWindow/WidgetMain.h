// $Revision: 1.7 $
/*!
  \file WidgetMain.h
  \brief
*/

#include "SmartWin.h"

class MDIChild;
using namespace SmartWin;

class WidgetMain
	: public WidgetFactory< WidgetWindow, WidgetMain >
{
private:
	WidgetMDIParentPtr mdiClient;
	std::vector < MDIChild * > mdiWindows;
	WidgetStatusBarPtr status;
	WidgetToolbarPtr toolbar;

	void sized( const SmartWin::WidgetSizedEventResult & sz );
	SmartUtil::tstring outputNumberOfMdiWidgets();
	void newWindow( WidgetButtonPtr btn );
	void toolbarClicked( WidgetToolbarPtr toolbar, unsigned btn );

public:
	void mdiClosed( MDIChild * mdi );
	void initAndCreate();
};
