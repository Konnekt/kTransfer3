// $Revision: 1.16 $
/*
  Copyright (c) 2005, Thomas Hansen
  All rights reserved.

  Redistribution and use in source and binary forms, with or without modification,
  are permitted provided that the following conditions are met:

	  * Redistributions of source code must retain the above copyright notice,
		this list of conditions and the following disclaimer.
	  * Redistributions in binary form must reproduce the above copyright notice, 
		this list of conditions and the following disclaimer in the documentation 
		and/or other materials provided with the distribution.
	  * Neither the name of the SmartWin++ nor the names of its contributors 
		may be used to endorse or promote products derived from this software 
		without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
  INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND 
  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, 
  OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#ifndef SoapResponse_H
#define SoapResponse_H

#include <sstream>
#include <fstream>
#include "SmartUtil.h"
#include "SmartNetwork.h" // To bring in HttpResponse
#include "SmartXML.h"
#include "xCeptionSoap.h"

namespace SmartSOAP
{
	// Forward declaring friends
	template< class Serializer, class SendObject, class ReturnObject >
	class SoapRequest;

	/// Encapsualtes a SOAP response
	/** This class is the return value from a SoapRequest.
	  * Use the SoapRequest class to build up a SOAP request and use this class to parse the result.
	  * Class is never ment for being used alone or constructed by any other means than by doing a "send" on a SoapRequest.
	  * Note that if you have a SOAP call which doesn't return anything at all, the serializer object doesn't actually HAVE to include
	  * the "deserialize" function but SHOULD call checkForExceptions() on the returned SoapResponse.
	  */
	template< class Serializer, class SendObject, class ReturnObject >
	class SoapResponse
	{
		friend class SoapRequest< Serializer, SendObject, ReturnObject >;
	private:
		SmartHTTP::HttpRequest::HttpResponsePtr itsHttpResponse;
		Serializer itsSerializer;

		// Since we use "lazy loading" of XML Document we make it "mutable" since then we can keep "logical const correctness"...
		mutable SmartXML::XmlDocument::XmlDocumentPtr itsXmlDocument;

		SoapResponse( SmartHTTP::HttpRequest::HttpResponsePtr httpResponse, Serializer serializer );

		SoapResponse( const SoapResponse & ); // Never implemented!!

		void loadXMLDocument() const;

	public:
		/// Verifies SOAP response is "good"
		/** This function will throw an xCeptionSoap exception if return value from server was either bad or encapsulated an exception.
		  */
		void checkForExceptions() const;

		/// Returns the return value from the SOAP response
		/** Dependant on the "deserialize" function of the Serializer object.
		  */
		ReturnObject getReturnValue() const;
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Implementation
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template< class Serializer, class SendObject, class ReturnObject >
	SoapResponse< Serializer, SendObject, ReturnObject >::SoapResponse( SmartHTTP::HttpRequest::HttpResponsePtr httpResponse, Serializer serializer )
		: itsHttpResponse( httpResponse ),
		itsSerializer( serializer )
	{}

	template< class Serializer, class SendObject, class ReturnObject >
	void SoapResponse< Serializer, SendObject, ReturnObject >::checkForExceptions() const
	{
		loadXMLDocument();
		if ( itsXmlDocument.get() == 0 )
			throw xCeptionSoap( "No XML document loaded" );

		if ( itsXmlDocument->getDocumentNode().get() == 0 )
			throw xCeptionSoap( "No XML document root node loaded" );

		if ( itsXmlDocument->getDocumentNode()->children_begin() == itsXmlDocument->getDocumentNode()->children_end() ||
			itsXmlDocument->getDocumentNode()->children_begin()->children_begin() == itsXmlDocument->getDocumentNode()->children_begin()->children_end() )
		{
			throw xCeptionSoap( "SOAP XML document node must have at least one child who have at least one child!" );
		}

		SmartXML::XmlNode & soapFault = ( * itsXmlDocument->getDocumentNode() )[0][0];
		if ( soapFault.getName() == _T( "Fault" ) )
		{
			throw xCeptionSoap( SmartUtil::AsciiGuaranteed::doConvert( soapFault[1].getContent(), SmartUtil::ConversionCodepage::UTF8 ) );
		}
		if ( itsXmlDocument->getDocumentNode()->getName() != _T( "Envelope" ) )
		{
			throw xCeptionSoap( "No valid XML document loaded from" );
		}
	}

	template< class Serializer, class SendObject, class ReturnObject >
	void SoapResponse< Serializer, SendObject, ReturnObject >::loadXMLDocument() const
	{
		// We make sure we only load it ONCE!!
		if ( itsXmlDocument.get() != 0 )
			return;
		itsXmlDocument = SmartXML::XmlDocument::createDocument( itsHttpResponse->getContentStream() );
		// TODO: Used for debugging purposes...
		//std::basic_ofstream<TCHAR> file( "\\Program Files\\PPCClient\\tmp.txt" );
		//file << itsXmlDocument->toString();
	}

	template< class Serializer, class SendObject, class ReturnObject >
	ReturnObject SoapResponse< Serializer, SendObject, ReturnObject >::getReturnValue() const
	{
		loadXMLDocument();
		checkForExceptions();
		return itsSerializer.deserialize( itsXmlDocument );
	}
}

#endif
