// $Revision: 1.6 $
/* Copyright (c) 2005 - Thomas Hansen
 * Part of the SmartWin++ library example code
 * Permission to use under the same terms as the actual library
 * The license to the library can be found at http://smartwinlib.org
 * License for library is Open Source and BSD
 */
#ifndef MainWindow_H
#define MainWindow_H

#include "SmartSOAP.h"
#include "SmartWin.h"
#include "WsdlParser.h"
#include "TextWindow.h"

using namespace SmartWin;
using namespace SmartUtil;
using namespace SmartXML;
using namespace SmartHTTP;

class MainWindow
	: public WidgetFactory< WidgetWindow, MainWindow >
{
	WidgetTextBoxPtr urlOfWsdl;
	WidgetButtonPtr fetchWsdl;
	WidgetStaticPtr description;

	void fetchWsdlFromUrl()
	{
		boost::tuples::tuple< SmartUtil::tstring, SmartUtil::tstring > retVal = WsdlParser::instance().fetchWsdlFromUrl( urlOfWsdl->getText() );

		// Creating "raw wsdl" window
		TextWindow * raw = new TextWindow();
		raw->init( _T( "Raw WSDL" ), retVal.get< 0 >(), RGB( 230, 230, 230 ), false );

		// Creating "proxy classes" window
		TextWindow * proxy = new TextWindow();
		proxy->init( _T( "Proxy classes" ), retVal.get< 1 >(), RGB( 255, 255, 255 ), true );
	}

	void fetchWsdlPushed( WidgetButtonPtr btn )
	{
		fetchWsdlFromUrl();
	}

public:
	void init( const SmartUtil::tstring & wsdl )
	{
		// Creating main window
		WidgetWindow::Seed mainSeed;
		mainSeed.style = WS_VISIBLE | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME;
		mainSeed.location.size.x = 420;
		mainSeed.location.size.y = 150;
		mainSeed.caption = _T( "SmartWin++, WSDL Parser" );
		createWindow( mainSeed );

		// Creating wsdl url text box
		WidgetTextBox::Seed txtSeed;
		txtSeed.location = sw::Rectangle( 20, 5, 300, 16 );
		txtSeed.style = WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_AUTOHSCROLL;
		txtSeed.caption = _T( "http : //localhost/Tester1Endpoint/CustomObjects.asmx?WSDL");
		urlOfWsdl = createTextBox( txtSeed );

		// Creating button to fetch wsdl
		WidgetButton::Seed fetchSeed;
		fetchSeed.location = sw::Rectangle( 325, 5, 80, 16 );
		fetchSeed.style |= WS_TABSTOP;
		fetchSeed.caption = _T( "Fetch WSDL" );
		fetchWsdl = createButton( fetchSeed );
		fetchWsdl->onClicked( & MainWindow::fetchWsdlPushed );

		// Creating "help file"
		WidgetStatic::Seed descSeed;
		descSeed.caption = _T( "Type in the URL of a WSDL document to have the proxy classes generated automatically for you!" );
		descSeed.location = sw::Rectangle( 20, 30, 380, 110 );
		description = createStatic( descSeed );

		// Setting initial focus
		urlOfWsdl->setFocus();
		urlOfWsdl->setSelection();

		if ( wsdl.size() != 0 )
		{
			urlOfWsdl->setText( wsdl );
			fetchWsdlFromUrl();
		}
	}
};

#endif
