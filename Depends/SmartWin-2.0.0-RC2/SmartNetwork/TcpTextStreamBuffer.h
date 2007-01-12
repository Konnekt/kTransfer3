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
#ifndef TcpStreamBuffer_H
#define TcpStreamBuffer_H

#include <streambuf>
#include <fstream>
#include "NetworkSystemHeaders.h"
#include "xCeptionSocket.h"
#include "SmartUtil.h"

namespace SmartNetwork
{
	/// Class encapsulating a streambuf which reads and writes across network boundaries
	/** Class is used in combination with TcpTextStream to give a std::iostream to read and write data across network boundaries.
	  * Jørgen Hovland from no.it.programmering.c++ has helped me out alot with getting the socket parts of this class to work!
	  * Thank you Jørgen! :)
	  * Note class is for TEXT reading across network boundaries and CANNOT read BINARY data!
	  * If used as a wchar_t stream it will convert data using UTF-8 standard.
	  */
	template< class Ch, class Tr = std::char_traits< Ch > >
	class TcpTextStreamBuffer
		: public std::basic_streambuf< Ch, Tr >
	{
	private:
		TCHAR * gai_strerror( int code )
		{
#ifdef __GNUC__
			return _T( "Undefined error when using socket library" );
#else
			return ::gai_strerror( code );
#endif
		}
		// Helper class to ensure WSAStartup is being called!!
		class InitWinSockets
		{
			InitWinSockets()
			{
				// Here we initialize the SOCKET library by calling WSAStartup.
				WORD wVersionRequested = MAKEWORD( 2, 2 );
				WSADATA wsaData;
				int err = WSAStartup( wVersionRequested, & wsaData );
				if ( err != 0 )
					throw xCeptionSocket( std::string( "Couldn't startup the socket library, error code was: " ) + boost::lexical_cast< std::string >( err ) );
			}

			~InitWinSockets()
			{
				WSACleanup();
			}

			InitWinSockets( const InitWinSockets & ); // Never implemented!!
		public:
			static InitWinSockets & instance()
			{
				static InitWinSockets retVal;
				return retVal;
			}
		};

		const static int BUFF_INPUT_SIZE = 8000;
		const static int BUFF_OUTPUT_SIZE = 8000; // After advice from Jørgen we keep the ouput buffer size SMALL to keep us from getting so many wouldblocks...
		Ch itsBuffOut[ BUFF_OUTPUT_SIZE ];
		Ch itsBuffIn[ BUFF_INPUT_SIZE + sizeof( Ch ) ];
		SOCKET itsSocket;
		unsigned int itsTimeout;
		SmartUtil::ConversionCodepage::Codepage itsCodepage;
		bool socketActive;

		void dumpTcpOutput()
		{
			// Output from pbase() to epptr()
			Ch * p = std::basic_streambuf< Ch, Tr >::pbase(); // a pointer to the beginning of the output buffer.

			int size, bytesSent;
			while ( p != std::basic_streambuf< Ch, Tr >::pptr() )
			{
				size = ( int )( std::basic_streambuf< Ch, Tr >::pptr() - p );

				SmartUtil::tstring str( p, size );
				std::string strChar = SmartUtil::AsciiGuaranteed::doConvert( str, itsCodepage );
				bytesSent = 0;
				while ( bytesSent < size )
				{
					// The buffer size might have increased due to the UNICODE conversion
					int idxBytesSent = send( itsSocket, strChar.substr( bytesSent ).c_str(), static_cast< int >( strChar.size() ) - bytesSent, 0 );
					if ( idxBytesSent == SOCKET_ERROR )
					{
						int error = WSAGetLastError();
						if ( error == WSAEWOULDBLOCK )
						{
							// Now we wait for the timeout to elapse...
							waitForReadyState( false );
							idxBytesSent = send( itsSocket, strChar.substr( bytesSent ).c_str(), static_cast< int >( strChar.size() ) - bytesSent, 0 );
							if ( idxBytesSent == SOCKET_ERROR )
							{
								error = WSAGetLastError();
								std::basic_stringstream< TCHAR > str;
								str << _T( "Socket error while waiting for select, error code was: " ) << error;
								MessageBox( 0, str.str().c_str(), 0, MB_OK );
								throw xCeptionSocket( "Couldn't recieve on socket" );
							}
						}
						else
						{
							std::basic_stringstream< TCHAR > str;
							str << _T( "Socket error while waiting for select, error code was: " ) << error;
							MessageBox( 0, str.str().c_str(), 0, MB_OK );
							throw xCeptionSocket( "Couldn't recieve on socket" );
						}
					}
					bytesSent += idxBytesSent;
				}
				p += size; // Remember the stuff we send is not neccesary the stuff we have in the buffer due to UNICODE conversion!!
			}

			// Set pptr() to buffer start
			setp( itsBuffOut, & itsBuffOut[BUFF_OUTPUT_SIZE] );
		}

		void waitForReadyState( bool readState )
		{
			// We wait until data is ready to be read/written to/from socket
			fd_set fd;
			FD_ZERO( & fd );
			FD_SET( itsSocket, & fd );
			timeval timeout;
			timeout.tv_sec = itsTimeout;
			timeout.tv_usec = 0;
			for (; ; )
			{
				int r = readState ? select( 0, & fd, 0, 0, & timeout ) : select( 0, 0, & fd, 0, & timeout );
				if ( r > 0 )
					break;
				else if ( !r && readState )
					throw xCeptionSocket( "Timeout expired while waiting for server to give us data" );
				else if ( !r && !readState )
					throw xCeptionSocket( "Timeout expired while waiting for server to be ready to recieve data" );
				else if ( SOCKET_ERROR == r )
				{
					int errorCode = WSAGetLastError();
					std::stringstream str;
					str << "Socket error while waiting for select, error code was: " << errorCode;
					throw xCeptionSocket( str.str() );
				}
			}
		}

	public:
		typedef typename Tr::int_type int_type;

		/// Initializes a socket connection to the given host on the given port
		void clientInit( const std::string & hostname, const std::string & port )
		{
			InitWinSockets::instance();
			if ( socketActive )
				throw xCeptionSocket( "Already initialised" );

			addrinfo hints;
			memset( & hints, 0, sizeof( addrinfo ) );
			hints.ai_family = PF_UNSPEC;
			hints.ai_socktype = SOCK_STREAM;
			hints.ai_flags = AI_PASSIVE;
			addrinfo * hostAddr( 0 );
			int err = ::getaddrinfo( hostname.c_str(), port.c_str(), & hints, & hostAddr ); // hostname might as well be an ip-address in any (un)supported protocol
			if ( err )
				throw xCeptionSocket( SmartUtil::AsciiGuaranteed::doConvert( this->gai_strerror( err ), SmartUtil::ConversionCodepage::ANSI ) );

			for ( addrinfo * AI = hostAddr; AI; AI = AI->ai_next )
			{
				SOCKET s = ::socket( AI->ai_family, AI->ai_socktype, AI->ai_protocol );
				if ( s == INVALID_SOCKET )
					continue; // Could be protocol not supported (bluetooth, ip, irda etc)

				static unsigned long yes( 1 );

				::ioctlsocket( s, FIONBIO, & yes );
				err = ::connect( s, ( const sockaddr * ) AI->ai_addr, static_cast< int >( AI->ai_addrlen ) );
				if ( !err )  // This probably only happens on connections to the same machine
				{
					itsSocket = s;
					socketActive = true;
					break;
				}
				else if ( WSAGetLastError() == WSAEINPROGRESS || WSAGetLastError() == WSAEWOULDBLOCK )
				{
					fd_set wfds;
					FD_ZERO( & wfds );
					FD_SET( s, & wfds );
					struct timeval tv;
					tv.tv_sec = itsTimeout;
					tv.tv_usec = 0;
					if ( 0 < ::select( static_cast< int >( s + 1 ), NULL, & wfds, NULL, & tv ) && ::send( s, "", 0, 0 ) == 0 ) // Select can return 1 if connection was refused etc. Send will find out. Any better way?
					{
						itsSocket = s;
						socketActive = true;
						break;
					}
				}
				::closesocket( s );
			}
			::freeaddrinfo( hostAddr );
			if ( !socketActive )
				throw xCeptionSocket( "Unable to connect" );
		}

		TcpTextStreamBuffer( unsigned int timeout, SmartUtil::ConversionCodepage::Codepage codepage )
			: std::basic_streambuf< Ch, Tr >(),
			itsTimeout( timeout ),
			itsCodepage( codepage )
		{
			setp( itsBuffOut, & itsBuffOut[BUFF_INPUT_SIZE] );
			setg( itsBuffIn, & itsBuffIn[BUFF_INPUT_SIZE], & itsBuffIn[BUFF_INPUT_SIZE] );
			socketActive = false;

			InitWinSockets::instance();
		}

		~TcpTextStreamBuffer()
		{
			dumpTcpOutput(); // If there is data to send, send it first.

			if ( socketActive )
			{
				closesocket( itsSocket );
			}
		}

		int sync()
		{
			dumpTcpOutput();
			return 0;
		}

		// This one WRITES to the socket
		int_type overflow( int_type c = Tr::eof() )
		{
			dumpTcpOutput();
			if ( Tr::eof() != c )
			{
				* ( std::basic_streambuf< Ch, Tr >::pptr() ) = c; // Put the overflow character in the buffer
				std::basic_streambuf< Ch, Tr >::pbump( 1 );
			}
			return Tr::not_eof( c );
		}

		int_type getData( unsigned int size, std::string & buffer )
		{
			// removing all characters from given buffer in case...
			buffer.clear();

			// then we read data from the socket!
			boost::scoped_array< char > buffIn( new char[size] );

			// We first try once WITHOUT the select...
			int n = recv( itsSocket, buffIn.get(), size, 0 );
			if ( 0 == n )
				return Tr::eof(); // Graceful shutdown of socket.
			if ( SOCKET_ERROR == n )
			{
				// Note this doesn't HAVE to be an error, it might also be that data wasn't ready to be sent (yet) from server
				int errorCode = WSAGetLastError();
				if ( errorCode == WSAEWOULDBLOCK )
				{
					// Now we wait for the timeout to elapse...
					waitForReadyState( true );

					// Then we try again to see if data s ready to be sent from server
					n = recv( itsSocket, buffIn.get(), size, 0 );
					if ( 0 == n )
						return Tr::eof(); // Graceful shutdown of socket.

					if ( SOCKET_ERROR == n )
					{
						// Now THIS IS an error, even if it's a WSAWOULDBLOCK since the timeout value has elapsed (or something else)
						errorCode = WSAGetLastError();
						std::basic_stringstream< TCHAR > str;
						str << _T( "Socket error while waiting for select, error code was: " ) << errorCode;
						throw xCeptionSocket( "Couldn't recieve on socket" );
					}
				}
				else
				{
					std::basic_stringstream< TCHAR > str;
					str << _T( "Socket error while waiting for select, error code was: " ) << errorCode;
					throw xCeptionSocket( "Couldn't recieve on socket" );
				}
			}
			buffer = std::string( buffIn.get(), n ); // Intentionally assuming buffIn contains size characters since logically we expect it to and if it doesn't we WANT to have an exception
			return n;
		}

		// This one reads from the socket
		int_type underflow()
		{
			dumpTcpOutput(); // If there is data to send, send it first.
			if ( std::basic_streambuf< Ch, Tr >::gptr() == std::basic_streambuf< Ch, Tr >::egptr() )
			{
				std::string buffIn;
				int_type n = getData( BUFF_INPUT_SIZE, buffIn );
				if ( n == Tr::eof() )
					return Tr::eof(); // No more data...!

				// Converting to current build (UNICODE if neccesary)
				SmartUtil::tstring str;
				if ( SmartUtil::Ascii2CurrentBuild::canConvert( buffIn, itsCodepage ) )
				{
					str = SmartUtil::Ascii2CurrentBuild::doConvert( buffIn, itsCodepage );
				}
				else
				{
					// This is probably caused by getting a leading byte with no trailing byte so we fetch one more char from the server for
					// 3 times (in cases of 32 bits wide charecters) and try to convert until we either make it before 3 more characters are
					// fetched or must "give up"...
					for ( int tryIdx = 0; tryIdx < 3; ++tryIdx )
					{
						std::string tmpBuffer;
						int_type n2 = getData( 1, tmpBuffer );
						if ( n2 == Tr::eof() )
							throw xCeptionSocket( "Garbage data from socket, got leading byte but no trailing byte from server due to eof after leading byte!" );

						// Intentionally assuming tmpBuffer contains 1 character since logically we expect it to and if it doesn't we WANT to have an exception
						buffIn.push_back( tmpBuffer[0] );
						if ( SmartUtil::Ascii2CurrentBuild::canConvert( buffIn, itsCodepage ) )
						{
							str = SmartUtil::Ascii2CurrentBuild::doConvert( buffIn, itsCodepage );
							break;
						}
					}
				}
//TODO: Uncommenting the following code will make the application fail at run-time:
//#ifdef __STDC_WANT_SECURE_LIB__
//				strcpy_s( itsBuffIn, sizeof( itsBuffIn[ BUFF_INPUT_SIZE + sizeof( Ch ) ] ), str.c_str() );
//#else
				_tcscpy( itsBuffIn, str.c_str() );
//#endif
				// Now when we HAVE converted the character string to wchar_t we need to update n since it might be shorter then what we actually recieved...
				n = static_cast< int_type >( str.size() );

				// Now ensure that the data is in the
				// last part of the buffer
				Ch * d = & itsBuffIn[ BUFF_INPUT_SIZE ];
				Ch * s = & itsBuffIn[ n ];

				if ( BUFF_INPUT_SIZE == n )
					d = itsBuffIn;
				else
				{
					// Move the data from the begining
					//  of the buffer to the end.
					while ( s != itsBuffIn )
					{
						d--;
						s--;
						* d = * s;
					}
				}
				setg( itsBuffIn, d, & itsBuffIn[BUFF_INPUT_SIZE] );
				return * d;
			}
			else
			{
				return Tr::eof();
			}
		}
	};
}

#endif
