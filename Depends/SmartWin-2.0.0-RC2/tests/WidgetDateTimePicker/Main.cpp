// $Revision: 1.13 $
/*!
  \file Main.cpp
  \brief Unit test for WidgetDateTimePicker

  It tries to instantiate all different forms of WidgetDateTimePicker and it also tries to use all functions.
*/

#include "SmartWin.h"
#include <vector>
#include <sstream>
#include <assert.h>
using namespace SmartWin;

// The "factory" version
class WidgetTest1
	: public WidgetFactory< WidgetWindow, WidgetTest1 >
{
	WidgetTextBoxPtr txt;
	WidgetDateTimePickerPtr date;

public:
	WidgetTest1()
	{}

	void dateChanged( WidgetDateTimePickerPtr date, const SYSTEMTIME & st )
	{
		std::basic_stringstream< TCHAR > str;
		// TODO: There is something fishy about basic_stringstream< TCHAR > and integer values...
		str << _T( "Year: " ) << st.wYear << _T( " Month: " )
			<< st.wMonth << _T( " Day: " )
			<< st.wDay << _T( " Hour: " )
			<< st.wHour << _T( " Minute: " )
			<< st.wMinute << _T( " Second: " )
			<< st.wSecond << _T( " Day of week: " )
			<< st.wDayOfWeek;
		txt->setText( str.str() );
	}

	void clicked( WidgetButtonPtr btn )
	{
		date->setEnabled( !date->getEnabled() );
		SYSTEMTIME st = date->getDateTime();
		std::basic_stringstream< TCHAR > str;
		str << _T( "Year: " ) << st.wYear << _T( " Month: " )
			<< st.wMonth << _T( " Day: " )
			<< st.wDay << _T( " Hour: " )
			<< st.wHour << _T( " Minute: " )
			<< st.wMinute << _T( " Second: " )
			<< st.wSecond << _T( " Day of week: " )
			<< st.wDayOfWeek;
		createMessageBox().show( str.str() );
	}

	void onClicked( WidgetDateTimePickerPtr date )
	{
		this->setText( _T( "Clicked" ) );
	}

	void onEnabled( WidgetDateTimePickerPtr date, bool isEnabled )
	{
		if ( isEnabled )
			this->setText( _T( "enabled" ) );
		else
			this->setText( _T( "NOT enabled" ) );
	}

	void focus( WidgetDateTimePickerPtr date )
	{
		this->setText( _T( "Focused" ) );
	}

	void killFocus( WidgetDateTimePickerPtr date )
	{
		this->setText( _T( "NOT Focused" ) );
	}

	bool keyPressed( WidgetDateTimePickerPtr date, int key )
	{
		SmartUtil::tstring tmp = this->getText();
		tmp.push_back( this->virtualKeyToChar( key ) );
		this->setText( tmp );
		return true;
	}

	void mouseMoved( WidgetDateTimePickerPtr date, const sw::MouseEventResult & result )
	{
		SmartUtil::tstring tmp = _T( "Mouse: " );
		tmp += boost::lexical_cast< SmartUtil::tstring >( result.pos.x );
		tmp += _T( ", " );
		tmp += boost::lexical_cast< SmartUtil::tstring >( result.pos.y );
		this->setText( tmp );
	}

	void painting( WidgetDateTimePickerPtr date, Canvas & canvas )
	{
		canvas.drawText( _T( "Thomas" ), sw::Rectangle( 0, 0, 100, 20 ), 0 );
	}

	void initAndCreate()
	{
		WidgetWindow::Seed windowCS;

		windowCS.location = SmartWin::Rectangle( 50, 50, 800, 800 );
		windowCS.caption = _T( "WidgetDateTimePicker compile and execute test" );
		windowCS.background = CreateSolidBrush( RGB( 0, 0, 125 ) );
		createWindow( windowCS );

		txt = createTextBox();
		txt->setBounds( 100, 400, 600, 50 );

		WidgetButtonPtr btn = createButton();
		btn->setBounds( 100, 350, 100, 40 );
		btn->setText( _T( "Retrieve date" ) );
		btn->onClicked( & WidgetTest1::clicked );

		WidgetDateTimePicker::Seed dateCS;

		dateCS.font = createFont( _T( "Comic Sans MS" ), 30, 10 );
		dateCS.location = SmartWin::Rectangle( 100, 100, 600, 50 );
		dateCS.format = _T( "yyyy.MM.dd HH:mm:ss" );
		date = createDateTimePicker( dateCS );
		date->onDateTimeChanged( & WidgetTest1::dateChanged );

		// Uncomment next line if you want to do custom painting of the whole control!!!!
		// Though it wont look very nice since the painting function doesn't really do anything intelligent at the moment... ;)
		// date->onPainting( &WidgetTest1::painting );
	}
};

int SmartWinMain( Application & app )
{
	// Widget (Factory) version
	WidgetTest1 * testWnd1 = new WidgetTest1;
	testWnd1->initAndCreate();
	return app.run();
}
