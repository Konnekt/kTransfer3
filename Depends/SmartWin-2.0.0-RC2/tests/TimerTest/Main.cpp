// $Revision: 1.5 $

#include "SmartWin.h"
using namespace SmartWin;

// The "factory" version
class WidgetTest1
	: public WidgetFactory< WidgetWindow, WidgetTest1 >
{
public:
	WidgetTest1()
	{}

	void timerTest( const CommandPtr & cmd )
	{
		SmartUtil::tstring text = getText();
		text = text.substr( 1 ) + text.substr( 0, 1 );
		setText( text );
		Command com ( _T( "Testing" ) );
		createTimer( & WidgetTest1::timerTest, 100, com );
	}

	void initAndCreate()
	{
		createWindow();
		this->setBounds( 0, 0, 400, 100 );

		setText( _T( "WidgetWindow Timer compile and execute test" ) );

		Command com( _T( "Testing" ) );
		createTimer( & WidgetTest1::timerTest, 100, com );
	}
};

// Unit test for WidgetSaveFile, tries to instantiate all different forms of WidgetSaveFile and also tries to use all functions
int SmartWinMain( Application & app )
{
	// Widget (Factory) version
	WidgetTest1 * testWnd1 = new WidgetTest1;
	testWnd1->initAndCreate();
	return app.run();
}
