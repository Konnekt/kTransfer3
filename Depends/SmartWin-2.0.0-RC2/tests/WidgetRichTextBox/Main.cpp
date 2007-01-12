// $Revision: 1.16 $
/*!
  \file Main.cpp
  \brief Unit test for WidgetRichTextBox

  It tries to instantiate all different forms of WidgetRichTextBox and also it tries to use all functions.
*/

#include "SmartWin.h"
#include <vector>
#include <assert.h>
using namespace SmartWin;

// The "factory" version
class WidgetTest1
	: public WidgetFactory< WidgetWindow, WidgetTest1 >
{
public:
	WidgetTest1()
	{}

	HRESULT selChanged( WidgetRichTextBoxPtr textField,
		LPARAM lPar, WPARAM wPar )
	{
		SELCHANGE * tmp = ( SELCHANGE * ) lPar;
		SmartUtil::tstring tmpTxt = textField->getSelection();
		this->setText( SmartUtil::tstring( _T( "New selection : " ) ) + tmpTxt + _T( ", " ) + boost::lexical_cast< SmartUtil::tstring >( ( int ) tmp->chrg.cpMin ) +
			SmartUtil::tstring( _T( ", " ) ) + boost::lexical_cast< SmartUtil::tstring >( ( int ) tmp->chrg.cpMax ) );
		textField->setBackgroundColor( RGB( rand()%255, rand()%255, rand()%255 ) );
		return 0;
	}

	// TODO : Fix this one since there's no such thing as a password richedit control
	void whenCreate( const SmartWin::Seed & cs )
	{
		WidgetRichTextBox::Seed richTextBoxCS;

		richTextBoxCS.font = createFont( _T( "Times New Roman" ), 28, 28, 8, ANSI_CHARSET, true, false, false, 0, OUT_DEFAULT_PRECIS );
		richTextBoxCS.caption = _T( "Yohooooo!!!" );
		richTextBoxCS.backgroundColor = RGB( rand()%255, rand()%255, rand()%255 );
		richTextBoxCS.location = SmartWin::Rectangle( 100, 100, 300, 300 );
		richTextBoxCS.scrollBarHorizontallyFlag = false;
		richTextBoxCS.scrollBarVerticallyFlag = false;
		WidgetRichTextBoxPtr textBox = createRichTextBox( richTextBoxCS );
		textBox->onKeyPressed( & WidgetTest1::richKeyPressed );

		WPARAM wPar = 0;
		LPARAM lPar = ( LPARAM ) ENM_SELCHANGE;
		textBox->sendWidgetMessage( textBox->handle(), ( UINT ) EM_SETEVENTMASK, wPar, lPar );
		textBox->onRaw( & WidgetTest1::selChanged, Message( WM_NOTIFY, EN_SELCHANGE ) );
	}

	bool richKeyPressed( WidgetRichTextBoxPtr textField, int key )
	{
		if ( this->getControlPressed() )
			return false;
		char tmp = this->virtualKeyToChar( key );
		if ( tmp == 0 || tmp == 0x08 || tmp == 13 )
			return false;
		return true;
	}

	void initAndCreate()
	{
		WidgetWindow::Seed windowCS;

		windowCS.location = SmartWin::Rectangle( 100, 100, 500, 500 );
		windowCS.background = CreateSolidBrush( RGB( 0, 0, 125 ) );
		windowCS.caption = _T( "WidgetRichTextBox compile and execute test" );
		onCreate( & WidgetTest1::whenCreate );
		createWindow( windowCS );
	}
};

int SmartWinMain( Application & app )
{
	// Widget (Factory) version
	WidgetTest1 * testWnd1 = new WidgetTest1;
	testWnd1->initAndCreate();
	return app.run();
}
