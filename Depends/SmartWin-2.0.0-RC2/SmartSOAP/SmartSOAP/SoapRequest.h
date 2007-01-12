// $Revision: 1.12 $
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
#ifndef SoapRequest_H
#define SoapRequest_H

#include <string>
#include <boost.h>
#include "SmartNetwork.h" // To bring in HttpRequest
#include "SoapResponse.h"

/// Contains SOAP main classes for the library SmartSOAP
namespace SmartSOAP
{
	/// SoapRequest, encapsulates a SOAP request
	/** The equivalent to SOAP as HttpRequest is to HTTP
	  * Use this class to do a SOAP function calling.
	  */
	template< class Serializer, class SendObject, class ReturnObject >
	class SoapRequest
	{
	private:
		SmartHTTP::HttpRequest itsHttpRequest;
		Serializer itsSerializer;
		const SendObject itsSendObject;

		SoapRequest( const SoapRequest & ); // Never implemented!!

	public:
		/// Return type of the send function
		typedef boost::shared_ptr< SoapResponse< Serializer, SendObject, ReturnObject > > SoapResponsePtr;

		/// Constructor creating a SOAP request
		/** The host should be e.g. "www.google.com" or "localhost".
		  * The postUrl should be e.g. "/MyApplication/default.asmx".
		  * The port should (normally) be 80.
		  * The serializer is a class which must expose two functions:
		  *   SmartUtil::tstring serialize( const SendObject & request );
		  *   ReturnObject deserialize( SmartXML::XmlDocument::XmlDocumentPtr xmlDocument ) const;
		  * Those functions will be called by the framework when needed and the serialize function is responsible for
		  * building up a string which will serve as the HTTP content of the request and must be in XML format while
		  * the deserialize function must parse the returned XML value from the SOAP response and return an object of type ReturnObject.
		  * The timeout defines how long time the framework will wait after submitting the SOAP request for the response before raising an exception.
		  */
		SoapRequest( const std::string & host,
			const SmartUtil::tstring & postUrl,
			const SmartUtil::tstring & soapAction,
			int port,
			Serializer serializer,
			const SendObject & sendObject,
			unsigned int timeout = 120 );

		SoapRequest( const std::string & connectionString,
			const SmartUtil::tstring & soapAction,
			int port,
			Serializer serializer,
			const SendObject & sendObject,
			unsigned int timeout = 120 );

		/// Sends the SoapRequest
		/** Returns a SoapResponse object after sending the SOAP request
		  */
		SoapResponsePtr send();
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Implementation
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template< class Serializer, class SendObject, class ReturnObject >
	SoapRequest< Serializer, SendObject, ReturnObject >::SoapRequest( const std::string & host,
		const SmartUtil::tstring & postUrl,
		const SmartUtil::tstring & soapAction,
		int port,
		Serializer serializer,
		const SendObject & sendObject,
		unsigned int timeout )
		: itsHttpRequest( host, postUrl, _T( "text/xml; charset=utf-8" ), boost::lexical_cast< std::string >( port ), timeout ),
		itsSerializer( serializer ),
		itsSendObject( sendObject )
	{
		itsHttpRequest.addHeader( soapAction );
	}

	template< class Serializer, class SendObject, class ReturnObject >
	SoapRequest< Serializer, SendObject, ReturnObject >::SoapRequest( const std::string & connectionString,
		const SmartUtil::tstring & soapAction,
		int port,
		Serializer serializer,
		const SendObject & sendObject,
		unsigned int timeout )
		: itsHttpRequest( connectionString, _T( "text/xml; charset=utf-8" ), boost::lexical_cast< std::string >( port ), timeout ),
		itsSerializer( serializer ),
		itsSendObject( sendObject )
	{
		itsHttpRequest.addHeader( soapAction );
	}

	template< class Serializer, class SendObject, class ReturnObject >
	typename SoapRequest< Serializer, SendObject, ReturnObject >::SoapResponsePtr SoapRequest< Serializer, SendObject, ReturnObject >::send()
	{
		SmartHTTP::HttpRequest::HttpResponsePtr response = itsHttpRequest.send( itsSerializer.serialize( itsSendObject ) );
		SoapResponsePtr retVal( new SoapResponse< Serializer, SendObject, ReturnObject >( response, itsSerializer ) );
		return retVal;
	}
}

#endif
