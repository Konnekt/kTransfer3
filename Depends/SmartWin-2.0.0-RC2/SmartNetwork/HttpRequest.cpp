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
#include <string>
#include <vector>
#include "TcpTextStream.h"
#include "HttpRequest.h"
#include "SmartUtil.h"

namespace SmartHTTP
{
HttpRequest::HttpRequest( const std::string & host, const SmartUtil::tstring & postUrl, const SmartUtil::tstring & contentType, const std::string & port, unsigned int timeout )
	: itsTcp( new SmartNetwork::TcpTextStream( host, port, timeout ) ),
	itsPostUrl( postUrl ),
	itsContentType( contentType ),
	itsHost( host )
{}

std::string getHost( const std::string & conn )
{
	size_t startOfHost = conn.find_first_of( ':' ) + 3;
	size_t endOfHost = conn.find_first_of( '/', startOfHost );
	std::string retVal = conn.substr( startOfHost, endOfHost - startOfHost );
	return retVal;
}

SmartUtil::tstring getPostPath( const std::string & conn )
{
	size_t startOfHost = conn.find_first_of( '/', conn.find_first_of( ':' ) + 4 );
	std::string retVal = conn.substr( startOfHost );
	return SmartUtil::Ascii2CurrentBuild::doConvert( retVal, SmartUtil::ConversionCodepage::ANSI );
}

HttpRequest::HttpRequest( const std::string & connectionString, const SmartUtil::tstring & contentType, const std::string & port, unsigned int timeout )
	: itsTcp( new SmartNetwork::TcpTextStream( getHost( connectionString ), port, timeout ) ),
	itsPostUrl( getPostPath( connectionString ) ),
	itsContentType( contentType ),
	itsHost( getHost( connectionString ) )
{
}

void HttpRequest::addHeader( const SmartUtil::tstring & header )
{
	itsHeaders.push_back( header );
}

HttpRequest::HttpResponsePtr HttpRequest::send( const SmartUtil::tstring & requestContent )
{
	// TODO: Optimize this whole function, it should among other things have an overload taking a stream for really big requests!!
	// How we should do it then with the Content-Length is another story!!
	std::vector< SmartUtil::tstring > headers;
	if ( itsPostUrl.find_first_of( _T( '?' ) ) != SmartUtil::tstring::npos )
		headers.push_back( SmartUtil::tstring( _T( "GET " ) ) + itsPostUrl + _T( " HTTP/1.0" ) );
	else
		headers.push_back( SmartUtil::tstring( _T( "POST " ) ) + itsPostUrl + _T( " HTTP/1.0" ) );

	// TODO: Optimize!! Really SLOW!!
	headers.push_back( SmartUtil::tstring( _T( "Host: " ) ) + SmartUtil::Ascii2CurrentBuild::doConvert( itsHost, SmartUtil::ConversionCodepage::ANSI ) );

	headers.push_back( SmartUtil::tstring( _T( "Content-Type: " ) ) + itsContentType );
	headers.push_back( _T( "Content-Length: " ) + boost::lexical_cast< SmartUtil::tstring >( static_cast< unsigned >( SmartUtil::AsciiGuaranteed::doConvert( requestContent, SmartUtil::ConversionCodepage::UTF8 ).size() ) ) );

	// A little bit of "advertizing"...
	headers.push_back( _T( "User-Agent: SmartSOAP v1.0 A part of the SmartWin++ library" ) );

	std::basic_stringstream< TCHAR > ss;
	for ( std::size_t x = 0; x < headers.size(); ++x )
	{
		ss << headers[x] << _T( "\r\n" );
	}
	for ( std::size_t x = 0; x < itsHeaders.size(); ++x )
	{
		ss << itsHeaders[x] << _T( "\r\n" );
	}
	ss << _T( "\r\n" );
	ss << requestContent;

	( * itsTcp ) << ss.str();
	return HttpResponsePtr( new HttpResponse( itsTcp ) );
}
}
