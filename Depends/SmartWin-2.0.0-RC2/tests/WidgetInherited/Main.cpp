// $Revision: 1.3 $
// Menu functions
//
// Unit tests try to instantiate all different forms and use all functions.
//

#include "SmartWin.h"
#include "InheritedTabSheet.h"

using namespace SmartWin;

// The "factory" version
class WidgetTest1
	: public WidgetFactory< WidgetWindow, WidgetTest1 >
{
	typedef InheritedTabSheet< WidgetTest1 > TabSheet;
	TabSheet * tab;
public:
	WidgetTest1()
	{}

	void sized( const WidgetSizedEventResult & res )
	{
		tab->layout(); ;
	}

	void selectedChanged( WidgetTabSheetPtr tab )
	{
		this->setText( tab->getSelectedHeader() );
	}

	void initAndCreate()
	{
		createWindow();
		this->setBounds( 100, 100, 400, 400 );
		tab = new TabSheet( this );
		tab->init();
		setText( _T( "WidgetInherited compile and execute test" ) );
		this->onSized( & WidgetTest1::sized );
		tab->onSelectionChanged( & WidgetTest1::selectedChanged );
	}
};

int SmartWinMain( Application & app )
{
	// Widget (Factory) version
	WidgetTest1 * testWnd1 = new WidgetTest1;
	testWnd1->initAndCreate();
	return app.run();
}
