// $Revision: 1.7 $

#include "SmartWin.h"

class WidgetMain;
using namespace SmartWin;

class MDIChild
	: public WidgetFactory< WidgetMDIChild, MDIChild, SmartWin::MessageMapPolicyMDIChildWidget >
{
	WidgetMain * itsParent;
	WidgetTextBoxPtr txt;
	bool closeMdi();
	void btnClicked( WidgetButtonPtr btn );
	void sized( const SmartWin::WidgetSizedEventResult & newSize );
public:
	// Even though we're using the SmartWin namespace we still have to prefix the WidgetMDIParent since
	// it's also a typedef in the WigetFactory class and the typedef will be tried during compilation first
	// and therefore we will get a compile time bug if we don't explicitly state which WidgetMDIParent
	// we want to use!
	// Also we can't use the typedef since it maps to WidgetMDIParent<WidgetMDI> and NOT WidetTest1!
	MDIChild( WidgetMain * parent, SmartWin::WidgetMDIParent< WidgetMain, SmartWin::MessageMapPolicyNormalWidget > * mdiParent );
};
