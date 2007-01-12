// $Revision: 1.10 $
/* Copyright (c) 2005 - Thomas Hansen
 * Part of the SmartWin++ library example code
 * Permission to use under the same terms as the actual library
 * The license to the library can be found at http://smartwinlib.org
 * License for library is Open Source and BSD
 */
#include "SmartWin.h"

struct UrlObject
{
	SmartUtil::tstring url;
	SmartUtil::tstring description;
};

struct SendObject
{
	SmartUtil::tstring query;
	SmartUtil::tstring key;
};

struct ReturnObject
{
	std::vector< UrlObject > retVal;
	SmartUtil::tstring rawRetVal;
};

class GoogleSerializer
{
public:
	SmartUtil::tstring serialize( const SendObject & query )
	{
		// We're using the SmartXML classes to serialize our request, but we strictly speaking don't have to do so!
		SmartXML::XmlDocument doc( _T( "Envelope" ), _T( "" ), _T( "soap" ) );
		SmartXML::XmlNode & rootNode = ( * doc.getDocumentNode() );
		rootNode.addNamespace( SmartXML::XmlNamespace( _T( "http : //schemas.xmlsoap.org/soap/envelope/"), _T("soap") ) );
		rootNode.addNamespace( SmartXML::XmlNamespace( _T( "http : //schemas.xmlsoap.org/soap/encoding/"), _T("soapenc") ) );
		rootNode.addNamespace( SmartXML::XmlNamespace( _T( "urn:GoogleSearch" ), _T( "tns" ) ) );
		rootNode.addNamespace( SmartXML::XmlNamespace( _T( "urn:GoogleSearch/encodedTypes" ), _T( "types" ) ) );
		rootNode.addNamespace( SmartXML::XmlNamespace( _T( "http : //www.w3.org/2001/XMLSchema-instance"), _T("xsi") ) );
		rootNode.addNamespace( SmartXML::XmlNamespace( _T( "http : //www.w3.org/2001/XMLSchema"), _T("xsd") ) );

		SmartXML::XmlNode & bodyNode = rootNode.addChild( _T( "Body" ), _T( "" ), _T( "soap" ) );
		bodyNode.addAttribute( SmartXML::XmlAttribute( & bodyNode, _T( "encodingStyle" ), _T( "http : //schemas.xmlsoap.org/soap/encoding/"), _T("soap") ) );

		SmartXML::XmlNode & doGoogleSearch = bodyNode.addChild( _T( "doGoogleSearch" ), _T( "" ), _T( "tns" ) );

		SmartXML::XmlNode & key = doGoogleSearch.addChild( _T( "key" ), query.key, _T( "" ) ); // TODO: Exchange key!!
		key.addAttribute( SmartXML::XmlAttribute( & key, _T( "type" ), _T( "xsd:string" ), _T( "xsi" ) ) );

		SmartXML::XmlNode & q = doGoogleSearch.addChild( _T( "q" ), query.query, _T( "" ) );
		q.addAttribute( SmartXML::XmlAttribute( & q, _T( "type" ), _T( "xsd:string" ), _T( "xsi" ) ) );

		SmartXML::XmlNode & start = doGoogleSearch.addChild( _T( "start" ), _T( "0" ), _T( "" ) );
		start.addAttribute( SmartXML::XmlAttribute( & start, _T( "type" ), _T( "xsd:int" ), _T( "xsi" ) ) );

		SmartXML::XmlNode & max = doGoogleSearch.addChild( _T( "maxResults" ), _T( "10" ), _T( "" ) );
		max.addAttribute( SmartXML::XmlAttribute( & max, _T( "type" ), _T( "xsd:int" ), _T( "xsi" ) ) );

		SmartXML::XmlNode & filter = doGoogleSearch.addChild( _T( "filter" ), _T( "false" ), _T( "" ) );
		filter.addAttribute( SmartXML::XmlAttribute( & filter, _T( "type" ), _T( "xsd:boolean" ), _T( "xsi" ) ) );

		SmartXML::XmlNode & restrict = doGoogleSearch.addChild( _T( "restrict" ), _T( "" ), _T( "" ) );
		restrict.addAttribute( SmartXML::XmlAttribute( & restrict, _T( "type" ), _T( "xsd:string" ), _T( "xsi" ) ) );

		SmartXML::XmlNode & safe = doGoogleSearch.addChild( _T( "safeSearch" ), _T( "false" ), _T( "" ) );
		safe.addAttribute( SmartXML::XmlAttribute( & safe, _T( "type" ), _T( "xsd:boolean" ), _T( "xsi" ) ) );

		SmartXML::XmlNode & lr = doGoogleSearch.addChild( _T( "lr" ), _T( "" ), _T( "" ) );
		lr.addAttribute( SmartXML::XmlAttribute( & lr, _T( "type" ), _T( "xsd:string" ), _T( "xsi" ) ) );

		SmartXML::XmlNode & ie = doGoogleSearch.addChild( _T( "ie" ), _T( "" ), _T( "" ) );
		ie.addAttribute( SmartXML::XmlAttribute( & ie, _T( "type" ), _T( "xsd:string" ), _T( "xsi" ) ) );

		SmartXML::XmlNode & oe = doGoogleSearch.addChild( _T( "oe" ), _T( "" ), _T( "" ) );
		oe.addAttribute( SmartXML::XmlAttribute( & oe, _T( "type" ), _T( "xsd:string" ), _T( "xsi" ) ) );

		return doc.toString();
	}

	ReturnObject deserialize( SmartXML::XmlDocument::XmlDocumentPtr document ) const
	{
		ReturnObject retVal;

		SmartXML::XmlNode & items = ( * document->getDocumentNode() )[0][0][0][5];
		for ( SmartXML::XmlNode::const_node_iterator idx = items.children_begin();
			idx != items.children_end();
			++idx )
		{
			SmartUtil::tstring url = ( * idx )[0].getContent();
			SmartUtil::tstring description = ( * idx )[6].getContent();
			UrlObject tmp;
			tmp.url = url;
			tmp.description = description;
			retVal.retVal.push_back( tmp );
		}

		retVal.rawRetVal = document->toString();
		return retVal;
	}
};

class MainWindow
	: public SmartWin::WidgetFactory< SmartWin::WidgetWindow, MainWindow >
{
	WidgetTextBoxPtr search;
	WidgetTextBoxPtr license;
	WidgetStaticPtr info;
	WidgetDataGridPtr datagrid;
	WidgetTextBoxPtr rawResponse;
	bool isSearching;

	unsigned long doSearch( SmartUtil::tstring & searchPhrase )
	{
		try
		{
			SendObject query;
			query.key = license->getText();
			query.query = searchPhrase;

			GoogleSerializer serializer;

			SmartSOAP::SoapRequest< GoogleSerializer, SendObject, ReturnObject > request(
				"api.google.com",  // Which host is our WebService located at
				_T( "/search/beta2" ), // What is the PATH to our WebService
				_T( "SOAPAction: \"urn:GoogleSearchAction\"" ), // What is the SOAPAction or basically the NAMESPACE of our WebService + the name of the function we're calling
				80, // Which TCP/IP port is the WebService listening on
				serializer, // The serializer/deserializer for our WebService call
				query // The object we actually want to send
				);
			SmartSOAP::SoapRequest< GoogleSerializer, SendObject, ReturnObject >::SoapResponsePtr response = request.send();
			ReturnObject result = response->getReturnValue();
			for ( std::vector< UrlObject >::const_iterator idx = result.retVal.begin();
				idx != result.retVal.end();
				++idx )
			{
				std::vector< SmartUtil::tstring > row;
				row.push_back( idx->url );
				row.push_back( idx->description );
				datagrid->insertRow( row );
			}
			rawResponse->setText( result.rawRetVal );
			isSearching = false; // To keep safe...
		}
		catch ( std::exception & err )
		{
			info->setText( SmartUtil::Ascii2CurrentBuild::doConvert( err.what(), SmartUtil::ConversionCodepage::ANSI ) );
			isSearching = false; // To keep safe...
		}
		catch ( ... )
		{
			isSearching = false; // To keep safe...
		}
		return 0;
	}

	bool textUpdated( WidgetTextBoxPtr box, int character )
	{
		if ( !isSearching && character == 13 ) // Carriage Return
		{
			isSearching = true;

			// Forking into the actual searching function...
			fork( box->getText(), & MainWindow::doSearch );

			// Resetting text of search box
			box->setText( _T( "Type your search phrase here!" ) );
			box->setFocus();
			box->setSelection();
		}
		return false;
	}

	void focus( WidgetTextBoxPtr box )
	{
#ifdef WINCE
		SmartWin::Application::instance().showSip( this, true );
#endif
		box->setSelection();
	}

	void killFocus( WidgetTextBoxPtr box )
	{
#ifdef WINCE
		SmartWin::Application::instance().showSip( this, false );
#endif
	}

public:
	void init()
	{
		// First creating our MAIN Container Widget
		createWindow();
		setBounds( 0, 0, 240, 320 ); // To mimic PocketPC size on DesktopSystems...
		setText( _T( "Google API SmartSOAP/SmartWin++ example..." ) );
		isSearching = false;

		sw::Rectangle cr( this->getClientAreaSize() );
		cr.pos.y += 28;

		// Then creating the search textbox
		WidgetTextBox::Seed seed;
		seed.font = sw::createFont( _T( "Verdana" ), 15, 8 );
		seed.caption = _T( "Type your search phrase here!" );
		seed.location = sw::Rectangle( 5, cr.pos.y, cr.size.x - 10, 20 );
		search = createTextBox( seed );
		search->onFocus( & MainWindow::focus );
		search->onKillFocus( & MainWindow::killFocus );
		search->setFocus();
		search->setSelection(); // Selects the whole range of text in the textbox
		search->onKeyPressed( & MainWindow::textUpdated );

		// Then creating the License Key text box
		seed.location.pos.y += 22;
		seed.caption = _T( "C6mZNWZQFHIb7X/qL9jq6cmoXxqHzqBI" );
		license = createTextBox( seed );

		// Creating "docs"
		WidgetStatic::Seed seedStatic;
		seedStatic.font = sw::createFont( _T( "Verdana" ), 12, 6 );
		seedStatic.caption = _T( "The above is my Google API key, please get your own if you submit lots of queries..." );
		seedStatic.location = seed.location;
		seedStatic.location.pos.y += 25;
		seedStatic.location.size.y = 50;
		info = createStatic( seedStatic );

		// Creating results
		datagrid = createDataGrid();
		cr = seedStatic.location;
		cr.pos.y += 55;
		cr.size.y = 80;
		datagrid->setBounds( cr );
		std::vector< SmartUtil::tstring > cols;
		cols.push_back( _T( "URL" ) );
		cols.push_back( _T( "Description" ) );
		datagrid->createColumns( cols );

		// Creating RawResponse textbox
		WidgetTextBox::Seed seedResult;
		seedResult.style |= WS_VSCROLL;
		rawResponse = createTextBox( seedResult );
		cr.pos.y += 85;
		cr.size.y -= 10;
		rawResponse->setBounds( cr );
		updateWidget();
	}
};
