// $Revision: 1.3 $

#include "SmartWin.h"

class WidgetTest
	: public SmartWin::WidgetFactory< SmartWin::WidgetWindow, WidgetTest >
{
public:
	WidgetTest()
	{}

	virtual void initAndCreate()
	{
		createWindow();
		setText( _T( "WidgetSaveFile compile and execute test" ) );
		WidgetSaveFile dlg = createSaveFile();
		dlg.addFilter( _T( "Text Files" ), _T( "*.txt" ) );
		dlg.addFilter( _T( "Data Files" ), _T( "*.dat" ) );
		dlg.addFilter( _T( "Registry Files" ), _T( "*.reg" ) );
		dlg.addFilter( _T( "All Files" ), _T( "*.*" ) );
		dlg.activeFilter( 3 );
		SmartUtil::tstring filePath = dlg.showDialog();
		SmartWin::WidgetMessageBox< SmartWin::Widget > msgBox( this );
		msgBox.show( filePath );
		close();
	}
};

// Unit test for WidgetSaveFile, tries to instantiate all different forms of WidgetSaveFile and also tries to use all functions
int SmartWinMain( SmartWin::Application & app )
{
	using namespace SmartWin;

	// First the "free" version...
	{
		WidgetSaveFileFree dlg;
		dlg.addFilter( _T( "Text Files" ), _T( "*.txt" ) );
		dlg.addFilter( _T( "Data Files" ), _T( "*.dat" ) );
		dlg.addFilter( _T( "Registry Files" ), _T( "*.reg" ) );
		dlg.addFilter( _T( "All Files" ), _T( "*.*" ) );
		dlg.activeFilter( 3 );
		SmartUtil::tstring filePath = dlg.showDialog();
		WidgetMessageBox< Widget > msgBox;
		msgBox.show( filePath );
	}
	{
		WidgetTest * testWnd = new WidgetTest;
		testWnd->initAndCreate();
		return app.run();
	}
}
