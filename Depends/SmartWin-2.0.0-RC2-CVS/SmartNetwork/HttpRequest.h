// $Revision: 1.5 $
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
#ifndef HttpRequest_H
#define HttpRequest_H

#include <string>
#include <vector>
#include <boost.h>
#include "TcpTextStream.h"
#include "HttpResponse.h"
#include "SmartUtil.h"

/// Contains HTTP helper classes for usage in SmartWin and SmartSOAP
namespace SmartHTTP
{
	/// Class for making an HTTP request.
	/** Class is tightly coupled with HttpResponse since the return value of making an HTTP request is an HttpResponse
	  * Use the constructor to create an object, add up some custom headers (or don't) and then use the send function to send the request
	  * and use the returned HttpResponse to parse your return value.
	  */
	class HttpRequest
	{
	private:
		boost::shared_ptr< SmartNetwork::TcpTextStream > itsTcp;
		std::vector< SmartUtil::tstring > itsHeaders;
		SmartUtil::tstring itsPostUrl;
		SmartUtil::tstring itsContentType;
		std::string itsHost;

		HttpRequest( const HttpRequest & rhs ); // Never implemented...!!

	public:
		/// The return type of calling the send function
		typedef boost::shared_ptr< HttpResponse > HttpResponsePtr;

		/// Constructor creating an HTTP request
		/** The host should be e.g. "www.google.com" or "localhost" and the postUrl should be e.g. "/MyApplication/default.aspx" or something.
		  * For most HTTP requests the port should be 80.
		  * The content type is the type of your request, e.g. for a SOAP calling this could be "text/xml"
		  * The timeout value defines how long time the request will wait after sending the request before it "gives up" getting a return value
		  * and raises an exception.
		  */
		HttpRequest( const std::string & host, const SmartUtil::tstring & postUrl, const SmartUtil::tstring & contentType, const std::string & port, unsigned int timeout );

		/// Parses the host and path automatically and adds up the parameters
		HttpRequest( const std::string & connectionString, const SmartUtil::tstring & contentType, const std::string & port, unsigned int timeout );

		/// Adds up an HTTP header
		/** By default only the "POST", "Host:", "Content-Length:" and the "Content-Type:" headers are included.
		  * If you need custom headers then use this function to add them up!
		  */
		void addHeader( const SmartUtil::tstring & header );

		/// Actually sends the request and returns the response in a HttpResponse object
		/** If the timeout periode elapses and the server is still unresponsive (the socket still isn't ready for receiving data) an exception
		  * will be raised.
		  */
		HttpResponsePtr send( const SmartUtil::tstring & requestContent );
	};
}

#endif
