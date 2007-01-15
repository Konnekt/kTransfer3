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
#ifndef HttpResponse_H
#define HttpResponse_H

#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <boost.h>
#include "TcpTextStream.h"
#include "SmartUtil.h"

namespace SmartHTTP
{
	// Forward declaring friends
	class HttpRequest;

	/// Class ancapsulating an HTTP response
	/** An HTTP response is the "return value" of submitting an HttpRequest and is basically worthless on its own or without
	  * creating and submitting an HTTP request beforehand.
	  */
	class HttpResponse
	{
		friend class HttpRequest;
	private:
		boost::shared_ptr< SmartNetwork::TcpTextStream > itsTcp;
		std::vector< SmartUtil::tstring > itsHeaders;

		HttpResponse( boost::shared_ptr< SmartNetwork::TcpTextStream > tcpStream );
		void fetchHeaders();

		HttpResponse( const HttpResponse & rhs ); // Never implemented...!!

	public:
		/// Returns the underlaying stream of the HTTP response
		/** Note that the returned stream is in "read only" mode
		  */
		std::basic_istream< TCHAR > & getContentStream();

		/// Returns true if HttpRequest is "good"
		/** If response is "good" or meaning it didn't generate a server exception or anything like that this function will return true
		  */
		bool isGood() const;
	};
}

#endif
