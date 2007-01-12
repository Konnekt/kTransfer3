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
		setText( _T( "WidgetMessageBox compile and execute test" ) );
		WidgetMessageBox msgBox = createMessageBox();
		WidgetMessageBox::RetVal retval = msgBox.show( _T( "Testing header" ), _T( "" ),
#ifdef MB_CANCELTRYCONTINUE
			WidgetMessageBox::BOX_CANCELTRYCONTINUE,
#else
			WidgetMessageBox::BOX_RETRYCANCEL,
#endif
			WidgetMessageBox::BOX_ICONINFORMATION );

		switch ( retval )
		{
		case WidgetMessageBox::RETBOX_CANCEL :
			createMessageBox().show( _T( "Cancel" ) );
			break;
		case WidgetMessageBox::RETBOX_TRYAGAIN :
			createMessageBox().show( _T( "Retry" ) );
			break;
		case WidgetMessageBox::RETBOX_CONTINUE :
			createMessageBox().show( _T( "Continue" ) );
			break;
		}
		close();
	}
};

// Unit test for WidgetLoadFile, tries to instantiate all different forms of WidgetLoadFile and also tries to use all functions
int SmartWinMain( SmartWin::Application & app )
{
	using namespace SmartWin;
	// First the "free" version...
	{
		WidgetMessageBoxFree msgBox1;
		msgBox1.show( _T( "Testing only body" ) );

		WidgetMessageBoxFree msgBox2;
		msgBox2.show( _T( "Testing body" ), _T( "Testing header" ) );

		WidgetMessageBoxFree msgBox3;
		msgBox3.show( _T( "Testing body" ), _T( "Testing header" ), WidgetMessageBoxFree::BOX_OK );

		WidgetMessageBoxFree msgBox4;
		msgBox4.show( _T( "Testing body" ), _T( "Testing header" ), WidgetMessageBoxFree::BOX_OK, WidgetMessageBoxFree::BOX_ICONEXCLAMATION );

		WidgetMessageBoxFree msgBox5;
		msgBox5.show( _T( "OK button, Icon Exclamation" ), _T( "Testing header" ),
			WidgetMessageBoxFree::BOX_OK, WidgetMessageBoxFree::BOX_ICONEXCLAMATION );

		WidgetMessageBoxFree msgBox6;
		msgBox6.show( _T( "OK/Cancel button, Icon Hand" ), _T( "Testing header" ),
			WidgetMessageBoxFree::BOX_OKCANCEL, WidgetMessageBoxFree::BOX_ICONHAND );

		WidgetMessageBoxFree msgBox7;
		msgBox7.show( _T( "BOX_ABORTRETRYIGNORE button, BOX_ICONQUESTION" ), _T( "Testing header" ),
			WidgetMessageBoxFree::BOX_ABORTRETRYIGNORE, WidgetMessageBoxFree::BOX_ICONQUESTION );

		WidgetMessageBoxFree msgBox8;
		msgBox8.show( _T( "BOX_YESNOCANCEL button, BOX_ICONASTERISK" ), _T( "Testing header" ),
			WidgetMessageBoxFree::BOX_YESNOCANCEL, WidgetMessageBoxFree::BOX_ICONASTERISK );

		WidgetMessageBoxFree msgBox9;
		msgBox9.show( _T( "BOX_YESNO button, BOX_ICONINFORMATION" ), _T( "Testing header" ),
			WidgetMessageBoxFree::BOX_YESNO, WidgetMessageBoxFree::BOX_ICONINFORMATION );

#ifdef MB_CANCELTRYCONTINUE
		WidgetMessageBoxFree msgBox10;
		msgBox10.show( _T( "BOX_CANCELTRYCONTINUE button, BOX_ICONSTOP" ), _T( "Testing header" ),
			WidgetMessageBoxFree::BOX_CANCELTRYCONTINUE, WidgetMessageBoxFree::BOX_ICONSTOP );

		WidgetMessageBoxFree msgBox11;
		msgBox11.show( _T( "BOX_RETRYCANCEL button, Default, should be BOX_ICONINFORMATION" ), _T( "Testing header" ),
			WidgetMessageBoxFree::BOX_CANCELTRYCONTINUE );
#endif
	}
	{
		WidgetTest * testWnd = new WidgetTest;
		testWnd->initAndCreate();
		return app.run();
	}
}
