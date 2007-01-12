// $Revision: 1.2 $
// Proxy classes autogenerated by SmartSOAPWsdlParser
// This is just an EXAMPLE of a WSDL parser and probably not very stable!!

#include "SmartSOAP.h"
#include "SmartUtil.h"
#include <memory>

struct HelloWorldInput;
struct HelloWorldResponseInput;
struct GetArrayOfStringsInput;
struct GetArrayOfStringsResponseInput;
struct ArrayOfStringInput;
struct GetCustomObjectInput;
struct TestGetStuffInput;
struct GetCustomObjectResponseInput;
struct TestReturnStuffInput;
struct ArrayOfTestReturnStuffContentsInput;
struct TestReturnStuffContentsInput;
struct GetDateTimeNowInput;
struct GetDateTimeNowResponseInput;
struct GetReallyAdvancedReturnValueInput;
struct ReallyAdvancedInputValueInput;
struct ArrayOfReallyAdvancedStuffInput;
struct ReallyAdvancedStuffInput;
struct ArrayOfReallyAdvancedStuffExtraInput;
struct ReallyAdvancedStuffExtraInput;
struct ArrayOfReallyAdvancedStuffExtraEmptyInput;
struct ReallyAdvancedStuffExtraEmptyInput;
struct GetReallyAdvancedReturnValueResponseInput;
struct ReallyAdvancedReturnValueInput;

struct ArrayOfStringInput
{
	std::vector< SmartUtil::null_pointer< SmartUtil::tstring > > string;

	void serialize( SmartXML::XmlNode & node ) const;
	void deserialize( const SmartXML::XmlNode & node );
};

struct TestGetStuffInput
{
	SmartUtil::null_pointer< SmartUtil::tstring > getStuffString;

	void serialize( SmartXML::XmlNode & node ) const;
	void deserialize( const SmartXML::XmlNode & node );
};

struct TestReturnStuffInput
{
	int intValue;
	SmartUtil::null_pointer< SmartUtil::tstring > stringValue;
	double decimalValue;
	SmartUtil::null_pointer< ArrayOfTestReturnStuffContentsInput > arrayOfContents;

	void serialize( SmartXML::XmlNode & node ) const;
	void deserialize( const SmartXML::XmlNode & node );
};

struct ArrayOfTestReturnStuffContentsInput
{
	std::vector< SmartUtil::null_pointer< TestReturnStuffContentsInput > > TestReturnStuffContents;

	void serialize( SmartXML::XmlNode & node ) const;
	void deserialize( const SmartXML::XmlNode & node );
};

struct TestReturnStuffContentsInput
{
	SmartUtil::null_pointer< SmartUtil::tstring > stringValue;

	void serialize( SmartXML::XmlNode & node ) const;
	void deserialize( const SmartXML::XmlNode & node );
};

struct ReallyAdvancedInputValueInput
{
	SmartUtil::null_pointer< ArrayOfReallyAdvancedStuffInput > arrayOfreallyAdvancedStuff;
	SmartUtil::null_pointer< ReallyAdvancedStuffInput > reallyAdvancedStuff;

	void serialize( SmartXML::XmlNode & node ) const;
	void deserialize( const SmartXML::XmlNode & node );
};

struct ArrayOfReallyAdvancedStuffInput
{
	std::vector< SmartUtil::null_pointer< ReallyAdvancedStuffInput > > ReallyAdvancedStuff;

	void serialize( SmartXML::XmlNode & node ) const;
	void deserialize( const SmartXML::XmlNode & node );
};

struct ReallyAdvancedStuffInput
{
	double decimalValue;
	int intValue;
	SmartUtil::null_pointer< SmartUtil::tstring > stringValue;
	float floatValue;
	double doubleValue;
	SmartUtil::DateTime dateTimeValue;
	SmartUtil::null_pointer< ArrayOfReallyAdvancedStuffExtraInput > extraValues;

	void serialize( SmartXML::XmlNode & node ) const;
	void deserialize( const SmartXML::XmlNode & node );
};

struct ArrayOfReallyAdvancedStuffExtraInput
{
	std::vector< SmartUtil::null_pointer< ReallyAdvancedStuffExtraInput > > ReallyAdvancedStuffExtra;

	void serialize( SmartXML::XmlNode & node ) const;
	void deserialize( const SmartXML::XmlNode & node );
};

struct ReallyAdvancedStuffExtraInput
{
	int y;
	SmartUtil::null_pointer< ArrayOfReallyAdvancedStuffExtraEmptyInput > tmp;
	SmartUtil::null_pointer< ReallyAdvancedStuffExtraEmptyInput > tmp2;

	void serialize( SmartXML::XmlNode & node ) const;
	void deserialize( const SmartXML::XmlNode & node );
};

struct ArrayOfReallyAdvancedStuffExtraEmptyInput
{
	std::vector< SmartUtil::null_pointer< ReallyAdvancedStuffExtraEmptyInput > > ReallyAdvancedStuffExtraEmpty;

	void serialize( SmartXML::XmlNode & node ) const;
	void deserialize( const SmartXML::XmlNode & node );
};

struct ReallyAdvancedStuffExtraEmptyInput
{
	void serialize( SmartXML::XmlNode & node ) const;
	void deserialize( const SmartXML::XmlNode & node );
};

struct ReallyAdvancedReturnValueInput
{
	SmartUtil::null_pointer< ArrayOfReallyAdvancedStuffInput > arrayOfreallyAdvancedStuff;
	SmartUtil::null_pointer< ReallyAdvancedStuffInput > reallyAdvancedStuff;

	void serialize( SmartXML::XmlNode & node ) const;
	void deserialize( const SmartXML::XmlNode & node );
};

struct HelloWorldInput
{
	int number;

	void serialize( SmartXML::XmlNode & node ) const;
	void deserialize( const SmartXML::XmlNode & node );
};

struct HelloWorldResponseInput
{
	SmartUtil::null_pointer< SmartUtil::tstring > HelloWorldResult;

	void serialize( SmartXML::XmlNode & node ) const;
	void deserialize( const SmartXML::XmlNode & node );
};

struct GetArrayOfStringsInput
{
	int howMany;
	SmartUtil::null_pointer< SmartUtil::tstring > whatShouldTheyContain;

	void serialize( SmartXML::XmlNode & node ) const;
	void deserialize( const SmartXML::XmlNode & node );
};

struct GetArrayOfStringsResponseInput
{
	SmartUtil::null_pointer< ArrayOfStringInput > GetArrayOfStringsResult;

	void serialize( SmartXML::XmlNode & node ) const;
	void deserialize( const SmartXML::XmlNode & node );
};

struct GetCustomObjectInput
{
	SmartUtil::null_pointer< TestGetStuffInput > getStuff;

	void serialize( SmartXML::XmlNode & node ) const;
	void deserialize( const SmartXML::XmlNode & node );
};

struct GetCustomObjectResponseInput
{
	SmartUtil::null_pointer< TestReturnStuffInput > GetCustomObjectResult;

	void serialize( SmartXML::XmlNode & node ) const;
	void deserialize( const SmartXML::XmlNode & node );
};

struct GetDateTimeNowInput
{
	int offset;

	void serialize( SmartXML::XmlNode & node ) const;
	void deserialize( const SmartXML::XmlNode & node );
};

struct GetDateTimeNowResponseInput
{
	SmartUtil::DateTime GetDateTimeNowResult;

	void serialize( SmartXML::XmlNode & node ) const;
	void deserialize( const SmartXML::XmlNode & node );
};

struct GetReallyAdvancedReturnValueInput
{
	SmartUtil::null_pointer< ReallyAdvancedInputValueInput > input;

	void serialize( SmartXML::XmlNode & node ) const;
	void deserialize( const SmartXML::XmlNode & node );
};

struct GetReallyAdvancedReturnValueResponseInput
{
	SmartUtil::null_pointer< ReallyAdvancedReturnValueInput > GetReallyAdvancedReturnValueResult;

	void serialize( SmartXML::XmlNode & node ) const;
	void deserialize( const SmartXML::XmlNode & node );
};

class CustomObjects
{
	struct HelloWorldSerializer
	{
		SmartUtil::tstring serialize( const HelloWorldInput & input )
		{
			SmartXML::XmlDocument doc( _T( "Envelope" ), _T( "" ), _T( "soap" ) );
			SmartXML::XmlNode & rootNode = ( * doc.getDocumentNode() );
			rootNode.addNamespace( SmartXML::XmlNamespace( _T( "http : //www.w3.org/2001/XMLSchema-instance"), _T("xsi") ) );
			rootNode.addNamespace( SmartXML::XmlNamespace( _T( "http : //www.w3.org/2001/XMLSchema"), _T("xsd") ) );
			rootNode.addNamespace( SmartXML::XmlNamespace( _T( "http : //schemas.xmlsoap.org/soap/envelope/"), _T("soap") ) );
			SmartXML::XmlNode & bodyNode = rootNode.addChild( _T( "Body" ), _T( "" ), _T( "soap" ) );
			SmartXML::XmlNode & functionNameNode = bodyNode.addChild( _T( "HelloWorld" ), _T( "" ), _T( "" ) );
			functionNameNode.addNamespace( SmartXML::XmlNamespace( _T( "http : //smartwin.org/SmartSOAP/WebServices/Test/CustomObject") ) );
			input.serialize( functionNameNode );
			return doc.toString();
		}

		HelloWorldResponseInput deserialize( SmartXML::XmlDocument::XmlDocumentPtr document ) const
		{
			HelloWorldResponseInput retVal;
			retVal.deserialize( ( * document->getDocumentNode() )[0][0] );
			return retVal;
		}
	};

	struct GetArrayOfStringsSerializer
	{
		SmartUtil::tstring serialize( const GetArrayOfStringsInput & input )
		{
			SmartXML::XmlDocument doc( _T( "Envelope" ), _T( "" ), _T( "soap" ) );
			SmartXML::XmlNode & rootNode = ( * doc.getDocumentNode() );
			rootNode.addNamespace( SmartXML::XmlNamespace( _T( "http : //www.w3.org/2001/XMLSchema-instance"), _T("xsi") ) );
			rootNode.addNamespace( SmartXML::XmlNamespace( _T( "http : //www.w3.org/2001/XMLSchema"), _T("xsd") ) );
			rootNode.addNamespace( SmartXML::XmlNamespace( _T( "http : //schemas.xmlsoap.org/soap/envelope/"), _T("soap") ) );
			SmartXML::XmlNode & bodyNode = rootNode.addChild( _T( "Body" ), _T( "" ), _T( "soap" ) );
			SmartXML::XmlNode & functionNameNode = bodyNode.addChild( _T( "GetArrayOfStrings" ), _T( "" ), _T( "" ) );
			functionNameNode.addNamespace( SmartXML::XmlNamespace( _T( "http : //smartwin.org/SmartSOAP/WebServices/Test/CustomObject") ) );
			input.serialize( functionNameNode );
			return doc.toString();
		}

		GetArrayOfStringsResponseInput deserialize( SmartXML::XmlDocument::XmlDocumentPtr document ) const
		{
			GetArrayOfStringsResponseInput retVal;
			retVal.deserialize( ( * document->getDocumentNode() )[0][0] );
			return retVal;
		}
	};

	struct GetCustomObjectSerializer
	{
		SmartUtil::tstring serialize( const GetCustomObjectInput & input )
		{
			SmartXML::XmlDocument doc( _T( "Envelope" ), _T( "" ), _T( "soap" ) );
			SmartXML::XmlNode & rootNode = ( * doc.getDocumentNode() );
			rootNode.addNamespace( SmartXML::XmlNamespace( _T( "http : //www.w3.org/2001/XMLSchema-instance"), _T("xsi") ) );
			rootNode.addNamespace( SmartXML::XmlNamespace( _T( "http : //www.w3.org/2001/XMLSchema"), _T("xsd") ) );
			rootNode.addNamespace( SmartXML::XmlNamespace( _T( "http : //schemas.xmlsoap.org/soap/envelope/"), _T("soap") ) );
			SmartXML::XmlNode & bodyNode = rootNode.addChild( _T( "Body" ), _T( "" ), _T( "soap" ) );
			SmartXML::XmlNode & functionNameNode = bodyNode.addChild( _T( "GetCustomObject" ), _T( "" ), _T( "" ) );
			functionNameNode.addNamespace( SmartXML::XmlNamespace( _T( "http : //smartwin.org/SmartSOAP/WebServices/Test/CustomObject") ) );
			input.serialize( functionNameNode );
			return doc.toString();
		}

		GetCustomObjectResponseInput deserialize( SmartXML::XmlDocument::XmlDocumentPtr document ) const
		{
			GetCustomObjectResponseInput retVal;
			retVal.deserialize( ( * document->getDocumentNode() )[0][0] );
			return retVal;
		}
	};

	struct GetDateTimeNowSerializer
	{
		SmartUtil::tstring serialize( const GetDateTimeNowInput & input )
		{
			SmartXML::XmlDocument doc( _T( "Envelope" ), _T( "" ), _T( "soap" ) );
			SmartXML::XmlNode & rootNode = ( * doc.getDocumentNode() );
			rootNode.addNamespace( SmartXML::XmlNamespace( _T( "http : //www.w3.org/2001/XMLSchema-instance"), _T("xsi") ) );
			rootNode.addNamespace( SmartXML::XmlNamespace( _T( "http : //www.w3.org/2001/XMLSchema"), _T("xsd") ) );
			rootNode.addNamespace( SmartXML::XmlNamespace( _T( "http : //schemas.xmlsoap.org/soap/envelope/"), _T("soap") ) );
			SmartXML::XmlNode & bodyNode = rootNode.addChild( _T( "Body" ), _T( "" ), _T( "soap" ) );
			SmartXML::XmlNode & functionNameNode = bodyNode.addChild( _T( "GetDateTimeNow" ), _T( "" ), _T( "" ) );
			functionNameNode.addNamespace( SmartXML::XmlNamespace( _T( "http : //smartwin.org/SmartSOAP/WebServices/Test/CustomObject") ) );
			input.serialize( functionNameNode );
			return doc.toString();
		}

		GetDateTimeNowResponseInput deserialize( SmartXML::XmlDocument::XmlDocumentPtr document ) const
		{
			GetDateTimeNowResponseInput retVal;
			retVal.deserialize( ( * document->getDocumentNode() )[0][0] );
			return retVal;
		}
	};

	struct GetReallyAdvancedReturnValueSerializer
	{
		SmartUtil::tstring serialize( const GetReallyAdvancedReturnValueInput & input )
		{
			SmartXML::XmlDocument doc( _T( "Envelope" ), _T( "" ), _T( "soap" ) );
			SmartXML::XmlNode & rootNode = ( * doc.getDocumentNode() );
			rootNode.addNamespace( SmartXML::XmlNamespace( _T( "http : //www.w3.org/2001/XMLSchema-instance"), _T("xsi") ) );
			rootNode.addNamespace( SmartXML::XmlNamespace( _T( "http : //www.w3.org/2001/XMLSchema"), _T("xsd") ) );
			rootNode.addNamespace( SmartXML::XmlNamespace( _T( "http : //schemas.xmlsoap.org/soap/envelope/"), _T("soap") ) );
			SmartXML::XmlNode & bodyNode = rootNode.addChild( _T( "Body" ), _T( "" ), _T( "soap" ) );
			SmartXML::XmlNode & functionNameNode = bodyNode.addChild( _T( "GetReallyAdvancedReturnValue" ), _T( "" ), _T( "" ) );
			functionNameNode.addNamespace( SmartXML::XmlNamespace( _T( "http : //smartwin.org/SmartSOAP/WebServices/Test/CustomObject") ) );
			input.serialize( functionNameNode );
			return doc.toString();
		}

		GetReallyAdvancedReturnValueResponseInput deserialize( SmartXML::XmlDocument::XmlDocumentPtr document ) const
		{
			GetReallyAdvancedReturnValueResponseInput retVal;
			retVal.deserialize( ( * document->getDocumentNode() )[0][0] );
			return retVal;
		}
	};

public:

	HelloWorldResponseInput HelloWorld( HelloWorldInput input )
	{
		HelloWorldSerializer serializer;
		SmartSOAP::SoapRequest< HelloWorldSerializer, HelloWorldInput, HelloWorldResponseInput > request(
			"http : //localhost/Tester1Endpoint/CustomObjects.asmx",
			_T( "SOAPAction : \"http : //smartwin.org/SmartSOAP/WebServices/Test/CustomObject/HelloWorld\""),
			80,
			serializer,
			input
			);
		SmartSOAP::SoapRequest< HelloWorldSerializer, HelloWorldInput, HelloWorldResponseInput >::SoapResponsePtr response = request.send();
		return response->getReturnValue();
	}

	GetArrayOfStringsResponseInput GetArrayOfStrings( GetArrayOfStringsInput input )
	{
		GetArrayOfStringsSerializer serializer;
		SmartSOAP::SoapRequest< GetArrayOfStringsSerializer, GetArrayOfStringsInput, GetArrayOfStringsResponseInput > request(
			"http : //localhost/Tester1Endpoint/CustomObjects.asmx",
			_T( "SOAPAction : \"http : //smartwin.org/SmartSOAP/WebServices/Test/CustomObject/GetArrayOfStrings\""),
			80,
			serializer,
			input
			);
		SmartSOAP::SoapRequest< GetArrayOfStringsSerializer, GetArrayOfStringsInput, GetArrayOfStringsResponseInput >::SoapResponsePtr response = request.send();
		return response->getReturnValue();
	}

	GetCustomObjectResponseInput GetCustomObject( GetCustomObjectInput input )
	{
		GetCustomObjectSerializer serializer;
		SmartSOAP::SoapRequest< GetCustomObjectSerializer, GetCustomObjectInput, GetCustomObjectResponseInput > request(
			"http : //localhost/Tester1Endpoint/CustomObjects.asmx",
			_T( "SOAPAction : \"http : //smartwin.org/SmartSOAP/WebServices/Test/CustomObject/GetCustomObject\""),
			80,
			serializer,
			input
			);
		SmartSOAP::SoapRequest< GetCustomObjectSerializer, GetCustomObjectInput, GetCustomObjectResponseInput >::SoapResponsePtr response = request.send();
		return response->getReturnValue();
	}

	GetDateTimeNowResponseInput GetDateTimeNow( GetDateTimeNowInput input )
	{
		GetDateTimeNowSerializer serializer;
		SmartSOAP::SoapRequest< GetDateTimeNowSerializer, GetDateTimeNowInput, GetDateTimeNowResponseInput > request(
			"http : //localhost/Tester1Endpoint/CustomObjects.asmx",
			_T( "SOAPAction : \"http : //smartwin.org/SmartSOAP/WebServices/Test/CustomObject/GetDateTimeNow\""),
			80,
			serializer,
			input
			);
		SmartSOAP::SoapRequest< GetDateTimeNowSerializer, GetDateTimeNowInput, GetDateTimeNowResponseInput >::SoapResponsePtr response = request.send();
		return response->getReturnValue();
	}

	GetReallyAdvancedReturnValueResponseInput GetReallyAdvancedReturnValue( GetReallyAdvancedReturnValueInput input )
	{
		GetReallyAdvancedReturnValueSerializer serializer;
		SmartSOAP::SoapRequest< GetReallyAdvancedReturnValueSerializer, GetReallyAdvancedReturnValueInput, GetReallyAdvancedReturnValueResponseInput > request(
			"http : //localhost/Tester1Endpoint/CustomObjects.asmx",
			_T( "SOAPAction : \"http : //smartwin.org/SmartSOAP/WebServices/Test/CustomObject/GetReallyAdvancedReturnValue\""),
			80,
			serializer,
			input
			);
		SmartSOAP::SoapRequest< GetReallyAdvancedReturnValueSerializer, GetReallyAdvancedReturnValueInput, GetReallyAdvancedReturnValueResponseInput >::SoapResponsePtr response = request.send();
		return response->getReturnValue();
	}
};

void ArrayOfStringInput::serialize( SmartXML::XmlNode & node ) const
{
	for ( std::vector< SmartUtil::null_pointer< SmartUtil::tstring > >::const_iterator idx = string.begin();
		 idx != string.end();
		 ++idx )
	{
		node.addChild( _T( "string" ), * * idx, _T( "" ) );
	}
}

void ArrayOfStringInput::deserialize( const SmartXML::XmlNode & node )
{
	int xmlNodeCounter = 0;

	if ( node[xmlNodeCounter].getName() == _T( "string" ) )
	{
		for ( SmartXML::XmlNode::const_node_iterator idx = node.children_begin();
			 idx != node.children_end();
			 ++idx )
		{
			string.push_back( SmartUtil::null_pointer< SmartUtil::tstring >( new SmartUtil::tstring( idx->getContent() ) ) );
		}
		++xmlNodeCounter;
	}
}

void TestGetStuffInput::serialize( SmartXML::XmlNode & node ) const
{
	if ( getStuffString.get() != 0 )
		node.addChild( _T( "getStuffString" ), * getStuffString, _T( "" ) );
}

void TestGetStuffInput::deserialize( const SmartXML::XmlNode & node )
{
	int xmlNodeCounter = 0;

	if ( node[xmlNodeCounter].getName() == _T( "getStuffString" ) )
		getStuffString = new SmartUtil::tstring( node[xmlNodeCounter++].getContent() );
}

void TestReturnStuffInput::serialize( SmartXML::XmlNode & node ) const
{
	node.addChild( _T( "intValue" ), boost::lexical_cast< SmartUtil::tstring >( intValue ), _T( "" ) );
	if ( stringValue.get() != 0 )
		node.addChild( _T( "stringValue" ), * stringValue, _T( "" ) );
	node.addChild( _T( "decimalValue" ), boost::lexical_cast< SmartUtil::tstring >( decimalValue ), _T( "" ) );
	if ( arrayOfContents.get() != 0 )
	{
		SmartXML::XmlNode & arrayOfContentsNode = node.addChild( "arrayOfContents", _T( "" ), "" );
		arrayOfContents->serialize( arrayOfContentsNode );
	}
}

void TestReturnStuffInput::deserialize( const SmartXML::XmlNode & node )
{
	int xmlNodeCounter = 0;

	intValue = boost::lexical_cast< int >( node[xmlNodeCounter++].getContent() );

	if ( node[xmlNodeCounter].getName() == _T( "stringValue" ) )
		stringValue = new SmartUtil::tstring( node[xmlNodeCounter++].getContent() );

	decimalValue = boost::lexical_cast< double >( node[xmlNodeCounter++].getContent() );

	if ( node[xmlNodeCounter].getName() == _T( "arrayOfContents" ) )
	{
		arrayOfContents = new ArrayOfTestReturnStuffContentsInput;
		arrayOfContents->deserialize( node[xmlNodeCounter++] );
	}
}

void ArrayOfTestReturnStuffContentsInput::serialize( SmartXML::XmlNode & node ) const
{
	for ( std::vector< SmartUtil::null_pointer< TestReturnStuffContentsInput > >::const_iterator idx = TestReturnStuffContents.begin();
		 idx != TestReturnStuffContents.end();
		 ++idx )
	{
		SmartXML::XmlNode & TestReturnStuffContentsNode = node.addChild( _T( "TestReturnStuffContents" ), _T( "" ), _T( "" ) );
		( * idx )->serialize( TestReturnStuffContentsNode );
	}
}

void ArrayOfTestReturnStuffContentsInput::deserialize( const SmartXML::XmlNode & node )
{
	int xmlNodeCounter = 0;

	if ( node[xmlNodeCounter].getName() == _T( "TestReturnStuffContents" ) )
	{
		for ( SmartXML::XmlNode::const_node_iterator idx = node.children_begin();
			 idx != node.children_end();
			 ++idx )
		{
			SmartUtil::null_pointer< TestReturnStuffContentsInput > tmp;
			const SmartXML::XmlAttribute * attribute = idx->findAttributeComplete( _T( "nil" ), _T( "http : //www.w3.org/2001/XMLSchema-instance"));
			if ( 0 == attribute || attribute->getContent() != _T( "true" ) )
			{
				tmp = new TestReturnStuffContentsInput;
				tmp->deserialize( * idx );
			}
			TestReturnStuffContents.push_back( tmp );
		}
		++xmlNodeCounter;
	}
}

void TestReturnStuffContentsInput::serialize( SmartXML::XmlNode & node ) const
{
	if ( stringValue.get() != 0 )
		node.addChild( _T( "stringValue" ), * stringValue, _T( "" ) );
}

void TestReturnStuffContentsInput::deserialize( const SmartXML::XmlNode & node )
{
	int xmlNodeCounter = 0;

	if ( node[xmlNodeCounter].getName() == _T( "stringValue" ) )
		stringValue = new SmartUtil::tstring( node[xmlNodeCounter++].getContent() );
}

void ReallyAdvancedInputValueInput::serialize( SmartXML::XmlNode & node ) const
{
	if ( arrayOfreallyAdvancedStuff.get() != 0 )
	{
		SmartXML::XmlNode & arrayOfreallyAdvancedStuffNode = node.addChild( "arrayOfreallyAdvancedStuff", _T( "" ), "" );
		arrayOfreallyAdvancedStuff->serialize( arrayOfreallyAdvancedStuffNode );
	}
	if ( reallyAdvancedStuff.get() != 0 )
	{
		SmartXML::XmlNode & reallyAdvancedStuffNode = node.addChild( "reallyAdvancedStuff", _T( "" ), "" );
		reallyAdvancedStuff->serialize( reallyAdvancedStuffNode );
	}
}

void ReallyAdvancedInputValueInput::deserialize( const SmartXML::XmlNode & node )
{
	int xmlNodeCounter = 0;

	if ( node[xmlNodeCounter].getName() == _T( "arrayOfreallyAdvancedStuff" ) )
	{
		arrayOfreallyAdvancedStuff = new ArrayOfReallyAdvancedStuffInput;
		arrayOfreallyAdvancedStuff->deserialize( node[xmlNodeCounter++] );
	}

	if ( node[xmlNodeCounter].getName() == _T( "reallyAdvancedStuff" ) )
	{
		reallyAdvancedStuff = new ReallyAdvancedStuffInput;
		reallyAdvancedStuff->deserialize( node[xmlNodeCounter++] );
	}
}

void ArrayOfReallyAdvancedStuffInput::serialize( SmartXML::XmlNode & node ) const
{
	for ( std::vector< SmartUtil::null_pointer< ReallyAdvancedStuffInput > >::const_iterator idx = ReallyAdvancedStuff.begin();
		 idx != ReallyAdvancedStuff.end();
		 ++idx )
	{
		SmartXML::XmlNode & ReallyAdvancedStuffNode = node.addChild( _T( "ReallyAdvancedStuff" ), _T( "" ), _T( "" ) );
		( * idx )->serialize( ReallyAdvancedStuffNode );
	}
}

void ArrayOfReallyAdvancedStuffInput::deserialize( const SmartXML::XmlNode & node )
{
	int xmlNodeCounter = 0;

	if ( node[xmlNodeCounter].getName() == _T( "ReallyAdvancedStuff" ) )
	{
		for ( SmartXML::XmlNode::const_node_iterator idx = node.children_begin();
			 idx != node.children_end();
			 ++idx )
		{
			SmartUtil::null_pointer< ReallyAdvancedStuffInput > tmp;
			const SmartXML::XmlAttribute * attribute = idx->findAttributeComplete( _T( "nil" ), _T( "http : //www.w3.org/2001/XMLSchema-instance"));
			if ( 0 == attribute || attribute->getContent() != _T( "true" ) )
			{
				tmp = new ReallyAdvancedStuffInput;
				tmp->deserialize( * idx );
			}
			ReallyAdvancedStuff.push_back( tmp );
		}
		++xmlNodeCounter;
	}
}

void ReallyAdvancedStuffInput::serialize( SmartXML::XmlNode & node ) const
{
	node.addChild( _T( "decimalValue" ), boost::lexical_cast< SmartUtil::tstring >( decimalValue ), _T( "" ) );
	node.addChild( _T( "intValue" ), boost::lexical_cast< SmartUtil::tstring >( intValue ), _T( "" ) );
	if ( stringValue.get() != 0 )
		node.addChild( _T( "stringValue" ), * stringValue, _T( "" ) );
	node.addChild( _T( "floatValue" ), boost::lexical_cast< SmartUtil::tstring >( floatValue ), _T( "" ) );
	node.addChild( _T( "doubleValue" ), boost::lexical_cast< SmartUtil::tstring >( doubleValue ), _T( "" ) );
	node.addChild( _T( "dateTimeValue" ), dateTimeValue.toString(), _T( "" ) );
	if ( extraValues.get() != 0 )
	{
		SmartXML::XmlNode & extraValuesNode = node.addChild( "extraValues", _T( "" ), "" );
		extraValues->serialize( extraValuesNode );
	}
}

void ReallyAdvancedStuffInput::deserialize( const SmartXML::XmlNode & node )
{
	int xmlNodeCounter = 0;

	decimalValue = boost::lexical_cast< double >( node[xmlNodeCounter++].getContent() );

	intValue = boost::lexical_cast< int >( node[xmlNodeCounter++].getContent() );

	if ( node[xmlNodeCounter].getName() == _T( "stringValue" ) )
		stringValue = new SmartUtil::tstring( node[xmlNodeCounter++].getContent() );

	floatValue = boost::lexical_cast< float >( node[xmlNodeCounter++].getContent() );

	doubleValue = boost::lexical_cast< double >( node[xmlNodeCounter++].getContent() );

	dateTimeValue = SmartUtil::DateTime( node[xmlNodeCounter++].getContent(), _T( "yyyy.MM.ddThh:mm:ss" ) );

	if ( node[xmlNodeCounter].getName() == _T( "extraValues" ) )
	{
		extraValues = new ArrayOfReallyAdvancedStuffExtraInput;
		extraValues->deserialize( node[xmlNodeCounter++] );
	}
}

void ArrayOfReallyAdvancedStuffExtraInput::serialize( SmartXML::XmlNode & node ) const
{
	for ( std::vector< SmartUtil::null_pointer< ReallyAdvancedStuffExtraInput > >::const_iterator idx = ReallyAdvancedStuffExtra.begin();
		 idx != ReallyAdvancedStuffExtra.end();
		 ++idx )
	{
		SmartXML::XmlNode & ReallyAdvancedStuffExtraNode = node.addChild( _T( "ReallyAdvancedStuffExtra" ), _T( "" ), _T( "" ) );
		( * idx )->serialize( ReallyAdvancedStuffExtraNode );
	}
}

void ArrayOfReallyAdvancedStuffExtraInput::deserialize( const SmartXML::XmlNode & node )
{
	int xmlNodeCounter = 0;

	if ( node[xmlNodeCounter].getName() == _T( "ReallyAdvancedStuffExtra" ) )
	{
		for ( SmartXML::XmlNode::const_node_iterator idx = node.children_begin();
			 idx != node.children_end();
			 ++idx )
		{
			SmartUtil::null_pointer< ReallyAdvancedStuffExtraInput > tmp;
			const SmartXML::XmlAttribute * attribute = idx->findAttributeComplete( _T( "nil" ), _T( "http : //www.w3.org/2001/XMLSchema-instance"));
			if ( 0 == attribute || attribute->getContent() != _T( "true" ) )
			{
				tmp = new ReallyAdvancedStuffExtraInput;
				tmp->deserialize( * idx );
			}
			ReallyAdvancedStuffExtra.push_back( tmp );
		}
		++xmlNodeCounter;
	}
}

void ReallyAdvancedStuffExtraInput::serialize( SmartXML::XmlNode & node ) const
{
	node.addChild( _T( "y" ), boost::lexical_cast< SmartUtil::tstring >( y ), _T( "" ) );
	if ( tmp.get() != 0 )
	{
		SmartXML::XmlNode & tmpNode = node.addChild( "tmp", _T( "" ), "" );
		tmp->serialize( tmpNode );
	}
	if ( tmp2.get() != 0 )
	{
		SmartXML::XmlNode & tmp2Node = node.addChild( "tmp2", _T( "" ), "" );
		tmp2->serialize( tmp2Node );
	}
}

void ReallyAdvancedStuffExtraInput::deserialize( const SmartXML::XmlNode & node )
{
	int xmlNodeCounter = 0;

	y = boost::lexical_cast< int >( node[xmlNodeCounter++].getContent() );

	if ( node[xmlNodeCounter].getName() == _T( "tmp" ) )
	{
		tmp = new ArrayOfReallyAdvancedStuffExtraEmptyInput;
		tmp->deserialize( node[xmlNodeCounter++] );
	}

	if ( node[xmlNodeCounter].getName() == _T( "tmp2" ) )
	{
		tmp2 = new ReallyAdvancedStuffExtraEmptyInput;
		tmp2->deserialize( node[xmlNodeCounter++] );
	}
}

void ArrayOfReallyAdvancedStuffExtraEmptyInput::serialize( SmartXML::XmlNode & node ) const
{
	for ( std::vector< SmartUtil::null_pointer< ReallyAdvancedStuffExtraEmptyInput > >::const_iterator idx = ReallyAdvancedStuffExtraEmpty.begin();
		 idx != ReallyAdvancedStuffExtraEmpty.end();
		 ++idx )
	{
		SmartXML::XmlNode & ReallyAdvancedStuffExtraEmptyNode = node.addChild( _T( "ReallyAdvancedStuffExtraEmpty" ), _T( "" ), _T( "" ) );
		( * idx )->serialize( ReallyAdvancedStuffExtraEmptyNode );
	}
}

void ArrayOfReallyAdvancedStuffExtraEmptyInput::deserialize( const SmartXML::XmlNode & node )
{
	int xmlNodeCounter = 0;

	if ( node[xmlNodeCounter].getName() == _T( "ReallyAdvancedStuffExtraEmpty" ) )
	{
		for ( SmartXML::XmlNode::const_node_iterator idx = node.children_begin();
			 idx != node.children_end();
			 ++idx )
		{
			SmartUtil::null_pointer< ReallyAdvancedStuffExtraEmptyInput > tmp;
			const SmartXML::XmlAttribute * attribute = idx->findAttributeComplete( _T( "nil" ), _T( "http : //www.w3.org/2001/XMLSchema-instance"));
			if ( 0 == attribute || attribute->getContent() != _T( "true" ) )
			{
				tmp = new ReallyAdvancedStuffExtraEmptyInput;
				tmp->deserialize( * idx );
			}
			ReallyAdvancedStuffExtraEmpty.push_back( tmp );
		}
		++xmlNodeCounter;
	}
}

void ReallyAdvancedStuffExtraEmptyInput::serialize( SmartXML::XmlNode & node ) const
{
}

void ReallyAdvancedStuffExtraEmptyInput::deserialize( const SmartXML::XmlNode & node )
{
	int xmlNodeCounter = 0;
}

void ReallyAdvancedReturnValueInput::serialize( SmartXML::XmlNode & node ) const
{
	if ( arrayOfreallyAdvancedStuff.get() != 0 )
	{
		SmartXML::XmlNode & arrayOfreallyAdvancedStuffNode = node.addChild( "arrayOfreallyAdvancedStuff", _T( "" ), "" );
		arrayOfreallyAdvancedStuff->serialize( arrayOfreallyAdvancedStuffNode );
	}
	if ( reallyAdvancedStuff.get() != 0 )
	{
		SmartXML::XmlNode & reallyAdvancedStuffNode = node.addChild( "reallyAdvancedStuff", _T( "" ), "" );
		reallyAdvancedStuff->serialize( reallyAdvancedStuffNode );
	}
}

void ReallyAdvancedReturnValueInput::deserialize( const SmartXML::XmlNode & node )
{
	int xmlNodeCounter = 0;

	if ( node[xmlNodeCounter].getName() == _T( "arrayOfreallyAdvancedStuff" ) )
	{
		arrayOfreallyAdvancedStuff = new ArrayOfReallyAdvancedStuffInput;
		arrayOfreallyAdvancedStuff->deserialize( node[xmlNodeCounter++] );
	}

	if ( node[xmlNodeCounter].getName() == _T( "reallyAdvancedStuff" ) )
	{
		reallyAdvancedStuff = new ReallyAdvancedStuffInput;
		reallyAdvancedStuff->deserialize( node[xmlNodeCounter++] );
	}
}

void HelloWorldInput::serialize( SmartXML::XmlNode & node ) const
{
	node.addChild( _T( "number" ), boost::lexical_cast< SmartUtil::tstring >( number ), _T( "" ) );
}

void HelloWorldInput::deserialize( const SmartXML::XmlNode & node )
{
	int xmlNodeCounter = 0;

	number = boost::lexical_cast< int >( node[xmlNodeCounter++].getContent() );
}

void HelloWorldResponseInput::serialize( SmartXML::XmlNode & node ) const
{
	if ( HelloWorldResult.get() != 0 )
		node.addChild( _T( "HelloWorldResult" ), * HelloWorldResult, _T( "" ) );
}

void HelloWorldResponseInput::deserialize( const SmartXML::XmlNode & node )
{
	int xmlNodeCounter = 0;

	if ( node[xmlNodeCounter].getName() == _T( "HelloWorldResult" ) )
		HelloWorldResult = new SmartUtil::tstring( node[xmlNodeCounter++].getContent() );
}

void GetArrayOfStringsInput::serialize( SmartXML::XmlNode & node ) const
{
	node.addChild( _T( "howMany" ), boost::lexical_cast< SmartUtil::tstring >( howMany ), _T( "" ) );
	if ( whatShouldTheyContain.get() != 0 )
		node.addChild( _T( "whatShouldTheyContain" ), * whatShouldTheyContain, _T( "" ) );
}

void GetArrayOfStringsInput::deserialize( const SmartXML::XmlNode & node )
{
	int xmlNodeCounter = 0;

	howMany = boost::lexical_cast< int >( node[xmlNodeCounter++].getContent() );

	if ( node[xmlNodeCounter].getName() == _T( "whatShouldTheyContain" ) )
		whatShouldTheyContain = new SmartUtil::tstring( node[xmlNodeCounter++].getContent() );
}

void GetArrayOfStringsResponseInput::serialize( SmartXML::XmlNode & node ) const
{
	if ( GetArrayOfStringsResult.get() != 0 )
	{
		SmartXML::XmlNode & GetArrayOfStringsResultNode = node.addChild( "GetArrayOfStringsResult", _T( "" ), "" );
		GetArrayOfStringsResult->serialize( GetArrayOfStringsResultNode );
	}
}

void GetArrayOfStringsResponseInput::deserialize( const SmartXML::XmlNode & node )
{
	int xmlNodeCounter = 0;

	if ( node[xmlNodeCounter].getName() == _T( "GetArrayOfStringsResult" ) )
	{
		GetArrayOfStringsResult = new ArrayOfStringInput;
		GetArrayOfStringsResult->deserialize( node[xmlNodeCounter++] );
	}
}

void GetCustomObjectInput::serialize( SmartXML::XmlNode & node ) const
{
	if ( getStuff.get() != 0 )
	{
		SmartXML::XmlNode & getStuffNode = node.addChild( "getStuff", _T( "" ), "" );
		getStuff->serialize( getStuffNode );
	}
}

void GetCustomObjectInput::deserialize( const SmartXML::XmlNode & node )
{
	int xmlNodeCounter = 0;

	if ( node[xmlNodeCounter].getName() == _T( "getStuff" ) )
	{
		getStuff = new TestGetStuffInput;
		getStuff->deserialize( node[xmlNodeCounter++] );
	}
}

void GetCustomObjectResponseInput::serialize( SmartXML::XmlNode & node ) const
{
	if ( GetCustomObjectResult.get() != 0 )
	{
		SmartXML::XmlNode & GetCustomObjectResultNode = node.addChild( "GetCustomObjectResult", _T( "" ), "" );
		GetCustomObjectResult->serialize( GetCustomObjectResultNode );
	}
}

void GetCustomObjectResponseInput::deserialize( const SmartXML::XmlNode & node )
{
	int xmlNodeCounter = 0;

	if ( node[xmlNodeCounter].getName() == _T( "GetCustomObjectResult" ) )
	{
		GetCustomObjectResult = new TestReturnStuffInput;
		GetCustomObjectResult->deserialize( node[xmlNodeCounter++] );
	}
}

void GetDateTimeNowInput::serialize( SmartXML::XmlNode & node ) const
{
	node.addChild( _T( "offset" ), boost::lexical_cast< SmartUtil::tstring >( offset ), _T( "" ) );
}

void GetDateTimeNowInput::deserialize( const SmartXML::XmlNode & node )
{
	int xmlNodeCounter = 0;

	offset = boost::lexical_cast< int >( node[xmlNodeCounter++].getContent() );
}

void GetDateTimeNowResponseInput::serialize( SmartXML::XmlNode & node ) const
{
	node.addChild( _T( "GetDateTimeNowResult" ), GetDateTimeNowResult.toString(), _T( "" ) );
}

void GetDateTimeNowResponseInput::deserialize( const SmartXML::XmlNode & node )
{
	int xmlNodeCounter = 0;

	GetDateTimeNowResult = SmartUtil::DateTime( node[xmlNodeCounter++].getContent(), _T( "yyyy.MM.ddThh:mm:ss" ) );
}

void GetReallyAdvancedReturnValueInput::serialize( SmartXML::XmlNode & node ) const
{
	if ( input.get() != 0 )
	{
		SmartXML::XmlNode & inputNode = node.addChild( "input", _T( "" ), "" );
		input->serialize( inputNode );
	}
}

void GetReallyAdvancedReturnValueInput::deserialize( const SmartXML::XmlNode & node )
{
	int xmlNodeCounter = 0;

	if ( node[xmlNodeCounter].getName() == _T( "input" ) )
	{
		input = new ReallyAdvancedInputValueInput;
		input->deserialize( node[xmlNodeCounter++] );
	}
}

void GetReallyAdvancedReturnValueResponseInput::serialize( SmartXML::XmlNode & node ) const
{
	if ( GetReallyAdvancedReturnValueResult.get() != 0 )
	{
		SmartXML::XmlNode & GetReallyAdvancedReturnValueResultNode = node.addChild( "GetReallyAdvancedReturnValueResult", _T( "" ), "" );
		GetReallyAdvancedReturnValueResult->serialize( GetReallyAdvancedReturnValueResultNode );
	}
}

void GetReallyAdvancedReturnValueResponseInput::deserialize( const SmartXML::XmlNode & node )
{
	int xmlNodeCounter = 0;

	if ( node[xmlNodeCounter].getName() == _T( "GetReallyAdvancedReturnValueResult" ) )
	{
		GetReallyAdvancedReturnValueResult = new ReallyAdvancedReturnValueInput;
		GetReallyAdvancedReturnValueResult->deserialize( node[xmlNodeCounter++] );
	}
}