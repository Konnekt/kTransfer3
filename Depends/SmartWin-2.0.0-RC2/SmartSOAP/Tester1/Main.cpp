// $Revision: 1.13 $

// Note that this is a "standalone" SmartSOAP application.
// It does't use ANYTHING from the GUI parts of SmartWin++ and only links to SmartSOAP.lib which is the pure "soap" parts of SmartWin++

#include <sstream>
#include "SmartSOAP.h"
#include "SmartXML.h"
#include "SmartUtil.h"

using namespace SmartSOAP;
using namespace SmartUtil;
using namespace SmartXML;
using namespace std;

// This is the ReturnValue from the WebService CustomObjects's HelloWorld function
class HelloWorldReturnValue
{
public:
	string helloWorldString;
};

// This is the object being "sent" to the HelloWorld function
class HelloWorldSender
{
public:
	int intValue;
};

class HelloWorldSerializer
{
public:
	string serialize( const HelloWorldSender & sender )
	{
		// We're using the SmartXML classes to serialize our request, but we strictly speaking don't have to do so!
		XmlDocument doc( "Envelope", "", "soap" );
		XmlNode & rootNode = ( * doc.getDocumentNode() );
		rootNode.addNamespace( XmlNamespace( "http : //www.w3.org/2001/XMLSchema-instance", "xsi" ) );
		rootNode.addNamespace( XmlNamespace( "http : //www.w3.org/2001/XMLSchema", "xsd" ) );
		rootNode.addNamespace( XmlNamespace( "http : //schemas.xmlsoap.org/soap/envelope/", "soap" ) );

		XmlNode & bodyNode = rootNode.addChild( "Body", "", "soap" );

		XmlNode & helloWorldNode = bodyNode.addChild( "HelloWorld", "", "" );
		helloWorldNode.addNamespace( XmlNamespace( "http : //smartwin.org/SmartSOAP/WebServices/Test/CustomObject" ) );
		helloWorldNode.addChild( "number", boost::lexical_cast< string >( sender.intValue ), "" );
		return doc.toString();
	}

	HelloWorldReturnValue deserialize( SmartXML::XmlDocument::XmlDocumentPtr document ) const
	{
		HelloWorldReturnValue retVal;

		// We're using the SmartXML to deserialize our response
		retVal.helloWorldString = ( * document->getDocumentNode() )[0][0][0].getContent();
		return retVal;
	}
};

// This is a "Proxy Function" which will call our HelloWorld WebService function abstracting away all the details of the WebService.
// You don't actually NEED one of these, but the code will be much easier to understand if you DO!
string helloWorld( int number )
{
	// First we create a HelloWorldSender which will be passed onto the WebService HelloWorld function
	HelloWorldSender sender;
	sender.intValue = number;

	// Then we must have a Serializer for serializing the Request and deserializing the Response
	HelloWorldSerializer serializer;

	// Then we create our SoapRequest
	SoapRequest< HelloWorldSerializer, HelloWorldSender, HelloWorldReturnValue > soap(
		"localhost",  // Which host is our WebService located at
		"/Tester1Endpoint/CustomObjects.asmx", // What is the PATH to our WebService
		"SOAPAction : \"http : //smartwin.org/SmartSOAP/WebServices/Test/CustomObject/HelloWorld\"", // What is the SOAPAction or basically the NAMESPACE of our WebService + the name of the function we're calling
		80, // Which TCP/IP port is the WebService listening on
		serializer, // The serializer/deserializer for our WebService call
		sender // The object we actually want to send
		);

	// Then we send the Request (or call the WebService function) and gets a SoapResponse back
	SoapRequest< HelloWorldSerializer, HelloWorldSender, HelloWorldReturnValue >::SoapResponsePtr response = soap.send();

	// Now our SoapResponse will contain a HelloWorldReturnValue which we can retrieve
	HelloWorldReturnValue returnFromServer = response->getReturnValue();

	// Now returning the returning string from our WebService
	return returnFromServer.helloWorldString;
}

class GetArrayOfStringsSender
{
public:
	int howMany;
	std::string whatShouldTheyContain;
};

class GetArrayOfStringsReturnValue
{
public:
	vector< string > returnedStrings;
};

class GetArrayOfStringsSerializer
{
public:
	string serialize( const GetArrayOfStringsSender & sender )
	{
		// We're using the SmartXML classes to serialize our request
		XmlDocument doc( "Envelope", "", "soap" );
		XmlNode & rootNode = ( * doc.getDocumentNode() );
		rootNode.addNamespace( XmlNamespace( "http : //www.w3.org/2001/XMLSchema-instance", "xsi" ) );
		rootNode.addNamespace( XmlNamespace( "http : //www.w3.org/2001/XMLSchema", "xsd" ) );
		rootNode.addNamespace( XmlNamespace( "http : //schemas.xmlsoap.org/soap/envelope/", "soap" ) );

		XmlNode & bodyNode = rootNode.addChild( "Body", "", "soap" );

		XmlNode & helloWorldNode = bodyNode.addChild( "GetArrayOfStrings", "", "" );
		helloWorldNode.addNamespace( XmlNamespace( "http : //smartwin.org/SmartSOAP/WebServices/Test/CustomObject" ) );
		helloWorldNode.addChild( "howMany", boost::lexical_cast< string >( sender.howMany ), "" );
		helloWorldNode.addChild( "whatShouldTheyContain", sender.whatShouldTheyContain, "" );
		return doc.toString();
	}

	GetArrayOfStringsReturnValue deserialize( SmartXML::XmlDocument::XmlDocumentPtr document ) const
	{
		GetArrayOfStringsReturnValue retVal;

		// We're using the SmartXML to deserialize our response
		for ( std::vector< XmlNode >::const_iterator stringIter = ( * document->getDocumentNode() )[0][0][0].children_begin();
			stringIter != ( * document->getDocumentNode() )[0][0][0].children_end();
			++stringIter )
		{
			retVal.returnedStrings.push_back( stringIter->getContent() );
		}
		return retVal;
	}
};

vector< string > getArrayOfStrings( const string & content, int howMany )
{
	GetArrayOfStringsSender sender;
	sender.howMany = howMany;
	sender.whatShouldTheyContain = content;

	// Then we must have a Serializer for serializing the Request and deserializing the Response
	GetArrayOfStringsSerializer serializer;

	// Then we create our SoapRequest
	SoapRequest< GetArrayOfStringsSerializer, GetArrayOfStringsSender, GetArrayOfStringsReturnValue > soap(
		"localhost",  // Which host is our WebService located at
		"/Tester1Endpoint/CustomObjects.asmx", // What is the PATH to our WebService
		"SOAPAction : \"http : //smartwin.org/SmartSOAP/WebServices/Test/CustomObject/GetArrayOfStrings\"", // What is the SOAPAction or basically the NAMESPACE of our WebService + the name of the function we're calling
		80, // Which TCP/IP port is the WebService listening on
		serializer, // The serializer/deserializer for our WebService call
		sender // The object we actually want to send
		);

	// Then we send the Request (or call the WebService function) and gets a SoapResponse back
	SoapRequest< GetArrayOfStringsSerializer, GetArrayOfStringsSender, GetArrayOfStringsReturnValue >::SoapResponsePtr response = soap.send();

	// Now our SoapResponse will contain a HelloWorldReturnValue which we can retrieve
	GetArrayOfStringsReturnValue returnFromServer = response->getReturnValue();

	// Now returning the returning string from our WebService
	return returnFromServer.returnedStrings;
}

class TestReturnStuffContents
{
public:
	string stringValue;
};

class TestReturnStuff
{
public:
	int intValue;
	string stringValue;
	double decimalValue;
	vector< TestReturnStuffContents > arrayOfContents;
};

class TestGetStuff
{
public:
	string getStuffString;
};

class GetCustomObjectSerializer
{
public:
	string serialize( const TestGetStuff & sender )
	{
		// We're using the SmartXML classes to serialize our request
		XmlDocument doc( "Envelope", "", "soap" );
		XmlNode & rootNode = ( * doc.getDocumentNode() );
		rootNode.addNamespace( XmlNamespace( "http : //www.w3.org/2001/XMLSchema-instance", "xsi" ) );
		rootNode.addNamespace( XmlNamespace( "http : //www.w3.org/2001/XMLSchema", "xsd" ) );
		rootNode.addNamespace( XmlNamespace( "http : //schemas.xmlsoap.org/soap/envelope/", "soap" ) );

		XmlNode & bodyNode = rootNode.addChild( "Body", "", "soap" );

		XmlNode & helloWorldNode = bodyNode.addChild( "GetCustomObject", "", "" );
		helloWorldNode.addNamespace( XmlNamespace( "http : //smartwin.org/SmartSOAP/WebServices/Test/CustomObject" ) );
		XmlNode & getStuffNode = helloWorldNode.addChild( "getStuff", "", "" );
		getStuffNode.addChild( "getStuffString", sender.getStuffString, "" );
		return doc.toString();
	}

	TestReturnStuff deserialize( SmartXML::XmlDocument::XmlDocumentPtr document ) const
	{
		TestReturnStuff retVal;

		// We're using the SmartXML to deserialize our response
		retVal.intValue = boost::lexical_cast< int >( ( * document->getDocumentNode() )[0][0][0][0].getContent() );
		retVal.stringValue = ( * document->getDocumentNode() )[0][0][0][1].getContent();
		retVal.decimalValue = boost::lexical_cast< double >( ( * document->getDocumentNode() )[0][0][0][2].getContent() );
		for ( std::vector< XmlNode >::const_iterator stringIter = ( * document->getDocumentNode() )[0][0][0][3].children_begin();
			stringIter != ( * document->getDocumentNode() )[0][0][0][3].children_end();
			++stringIter )
		{
			TestReturnStuffContents tmp;
			tmp.stringValue = ( * stringIter )[0].getContent();
			retVal.arrayOfContents.push_back( tmp );
		}
		return retVal;
	}
};

TestReturnStuff getCustomObject( const TestGetStuff & getStuff )
{
	// Then we must have a Serializer for serializing the Request and deserializing the Response
	GetCustomObjectSerializer serializer;

	// Then we create our SoapRequest
	SoapRequest< GetCustomObjectSerializer, TestGetStuff, TestReturnStuff > soap(
		"localhost",
		"/Tester1Endpoint/CustomObjects.asmx",
		"SOAPAction : \"http : //smartwin.org/SmartSOAP/WebServices/Test/CustomObject/GetCustomObject\"",
		80,
		serializer,
		getStuff
		);

	// Then we send the Request (or call the WebService function) and gets a SoapResponse back
	SoapRequest< GetCustomObjectSerializer, TestGetStuff, TestReturnStuff >::SoapResponsePtr response = soap.send();

	// Now our SoapResponse will contain a HelloWorldReturnValue which we can retrieve
	TestReturnStuff returnFromServer = response->getReturnValue();

	// Now returning the returning string from our WebService
	return returnFromServer;
}

class GetDateTimeNowRequest
{
public:
	int offset;
};

class GetDateTimeNowResponse
{
public:
	DateTime dateTime;
};

class GetDateTimeNowSerializer
{
public:
	string serialize( const GetDateTimeNowRequest & sender )
	{
		// We're using the SmartXML classes to serialize our request
		XmlDocument doc( "Envelope", "", "soap" );
		XmlNode & rootNode = * doc.getDocumentNode();
		rootNode.addNamespace( XmlNamespace( "http : //www.w3.org/2001/XMLSchema-instance", "xsi" ) );
		rootNode.addNamespace( XmlNamespace( "http : //www.w3.org/2001/XMLSchema", "xsd" ) );
		rootNode.addNamespace( XmlNamespace( "http : //schemas.xmlsoap.org/soap/envelope/", "soap" ) );

		XmlNode & bodyNode = rootNode.addChild( "Body", "", "soap" );

		XmlNode & getDateTimeNow = bodyNode.addChild( "GetDateTimeNow", "", "" );
		getDateTimeNow.addNamespace( XmlNamespace( "http : //smartwin.org/SmartSOAP/WebServices/Test/CustomObject" ) );
		getDateTimeNow.addChild( "offset", boost::lexical_cast< string >( sender.offset ), "" );
		return doc.toString();
	}

	GetDateTimeNowResponse deserialize( SmartXML::XmlDocument::XmlDocumentPtr document ) const
	{
		GetDateTimeNowResponse retVal;

		// We're using the SmartXML to deserialize our response
		retVal.dateTime = DateTime( ( * document->getDocumentNode() )[0][0][0].getContent(), "yyyy.MM.dd HH:mm:ss" );
		return retVal;
	}
};

DateTime getDateTimeNowResponse( int offset )
{
	// Then we must have a Serializer for serializing the Request and deserializing the Response
	GetDateTimeNowSerializer serializer;

	GetDateTimeNowRequest tmp;
	tmp.offset = offset;

	// Then we create our SoapRequest
	SoapRequest< GetDateTimeNowSerializer, GetDateTimeNowRequest, GetDateTimeNowResponse > soap(
		"localhost",
		"/Tester1Endpoint/CustomObjects.asmx",
		"SOAPAction : \"http : //smartwin.org/SmartSOAP/WebServices/Test/CustomObject/GetDateTimeNow\"",
		80,
		serializer,
		tmp
		);

	// Then we send the Request (or call the WebService function) and gets a SoapResponse back
	SoapRequest< GetDateTimeNowSerializer, GetDateTimeNowRequest, GetDateTimeNowResponse >::SoapResponsePtr response = soap.send();

	// Now our SoapResponse will contain a HelloWorldReturnValue which we can retrieve
	GetDateTimeNowResponse returnFromServer = response->getReturnValue();

	// Now returning the returning string from our WebService
	return returnFromServer.dateTime;
}

int main()
{
	cout << "Calling our WebService with the value of: 2" << endl;
	cout << "WebService returns: " << endl;
	cout << helloWorld( 2 ) << endl << endl;

	cout << "Calling our WebService with the value of: 5" << endl;
	cout << "WebService returns: " << endl;
	cout << helloWorld( 5 ) << endl << endl;

	cout << "Now we're gonna call the WebService with the value 15, this will trigger an Exception which the SmartSOAP will parse for us and 'convert' into an xCeptionSoap..." << endl;
	try
	{
		helloWorld( 15 ); // Triggers an Exception
	}
	catch ( xCeptionSoap & err )
	{
		cout << "The message from the server was:" << endl << endl << err.what() << endl;
	}

	cout << endl << endl << "Then we're gonna call a bit more 'advanced' WebService function" << endl;
	cout << "Calling GetArrayOfStrings with 5, 'thomas'" << endl;
	cout << "Return value was" << endl;
	vector< string > strings1 = getArrayOfStrings( "thomas", 5 );
	for ( vector< string >::const_iterator idxIter = strings1.begin();
		idxIter != strings1.end();
		++idxIter )
	{
		cout << "\t\t" << * idxIter << endl;
	}

	cout << endl << endl << "Then we're call a function taking a custom object returning another custom object" << endl;
	TestGetStuff getStuff;
	getStuff.getStuffString = "Tjobing";
	TestReturnStuff returnStuff = getCustomObject( getStuff );
	cout << "Return value was: " << endl;
	cout << returnStuff.intValue << endl << returnStuff.stringValue << endl << returnStuff.decimalValue << endl;
	for ( std::vector< TestReturnStuffContents >::const_iterator iter = returnStuff.arrayOfContents.begin();
		iter != returnStuff.arrayOfContents.end();
		++iter )
	{
		cout << "\t\t" << iter->stringValue << endl;
	}

	cout << endl << endl << "Then we're call a function returnung a DateTime object" << endl;
	DateTime date = getDateTimeNowResponse( 1 );
	cout << "Return was: " << date.toDateString( "dd.MM.yyyy" ) << " " << date.toTimeString( "HH:mm:ss" );

	string g;
	cin >> g;

	return ERROR_SUCCESS;
}
