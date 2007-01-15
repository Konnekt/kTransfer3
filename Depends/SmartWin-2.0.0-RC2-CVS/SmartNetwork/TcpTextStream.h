// $Revision: 1.3 $
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
#ifndef TcpStream_H
#define TcpStream_H

#include <string>
#include <iostream>
#include <boost.h>
#include "TcpTextStreamBuffer.h"
#include "xCeptionSocket.h"
#include "SmartUtil.h"

/// Contains network helper classes used in e.g. SmartSOAP
namespace SmartNetwork
{
	/// Class for encapsulating a Socket Stream
	/** Use the class as a normal "std::iostream" object.
	  * You can write to the stream using the "insertion operator" (<<) and read from the stream using the "extraction operator". (>>)
	  * Class encapsulates a SOCKET and functions like a both readable and writable stream.
	  * However logic on the server side will always make the stream either ready for "writing" or "reading".
	  * Like for instance an HTTP request will always expect the client to first send a (complete) HTTP request and then read the (complete) response
	  * before trying to send another request on the same socket!
	  */
	class TcpTextStream
		: public std::basic_iostream< TCHAR >
	{
		TcpTextStreamBuffer< TCHAR > itsTcpStreamBuffer;

	public:
		/// Constructor creating a TcpTextStream
		/** The serverHostName should be e.g. "www.google.com" or "localhost", the port should be the port you expect to use and the timeout
		  * is the amount of seconds we wait after starting a "read action" before we give up and throws an exception.
		  * Stream is synchronous meaning if you need asynchronous access you need to wrap it yourself!
		  */
		TcpTextStream( const std::string & serverHostAddress, const std::string & port, unsigned int timeout,
			SmartUtil::ConversionCodepage::Codepage codepage = SmartUtil::ConversionCodepage::UTF8 );
	};
}

#endif
