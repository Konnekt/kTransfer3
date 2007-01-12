// $Revision: 1.4 $
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
#include <sstream>
#include "HttpResponse.h"

namespace SmartHTTP
{
HttpResponse::HttpResponse( boost::shared_ptr< SmartNetwork::TcpTextStream > tcpStream )
	: itsTcp( tcpStream )
{
	fetchHeaders();
}

void HttpResponse::fetchHeaders()
{
	unsigned int contentLength = static_cast< unsigned int >( - 1 );
	while ( true )
	{
		SmartUtil::tstring buffer;
		std::getline< TCHAR >( * itsTcp, buffer, _T( '\r' ) );
		itsTcp->get(); /* Eat the '\n' character */
		if ( buffer.size() == 0 )
			break; // When we come to the empty line HTTP Headers are finished fetched!!!
		itsHeaders.push_back( buffer );
		if ( buffer.find( _T( "Content-Length:" ) ) != SmartUtil::tstring::npos )
		{
			contentLength = boost::lexical_cast< unsigned int >( buffer.substr( 16 ) ); // Space before the number starts...
		}
	}
}

std::basic_istream< TCHAR > & HttpResponse::getContentStream()
{
	return * itsTcp;
}

bool HttpResponse::isGood() const
{
	// The first header is the "HTTP/1.1 200 OK" header (if success)
	if ( itsHeaders.size() == 0 || itsHeaders[0].size() < 8 )
		return false; // HTTP/1.1 is 8 letters...

	size_t beginOfCode = itsHeaders[0].find( _T( " " ) );
	if ( beginOfCode == std::string::npos )
		return false;

	SmartUtil::tstring value = itsHeaders[0].substr( beginOfCode + 1, 3 );
	return value == _T( "200" );
}
}
