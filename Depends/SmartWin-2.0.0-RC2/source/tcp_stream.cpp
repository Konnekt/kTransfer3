// $Revision: 1.2 $
#include "../include/io/tcp_stream.h"
#include "string"
#include <iostream>
#include <sstream>
using namespace std;

/*-----------------------------------------------------------------*/

// Initalized static member variables

bool tcp_streambuf< char >::iws_done = false; // Initalized false

map< int, SOCKET > tcp_streambuf< char >::itsListen_sockets;

/*-----------------------------------------------------------------*/

/*-----------------------------------------------------------------*/

template< class Ch, class Tr >
bool tcp_streambuf< Ch, Tr >::init_win_sockets()
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	if ( iws_done ) return( true );

	wVersionRequested = MAKEWORD( 2, 2 );

	err = WSAStartup( wVersionRequested, & wsaData );
	if ( err != 0 )
	{
		/* Tell the user that we could not find a usable */
		/* WinSock DLL.                                  */
		return( false );
	}
	iws_done = true;
	return( true );
}

/*-----------------------------------------------------------------*/

//  Error codes are explained at:
//  http://msdn.microsoft.com/library/default.asp?url=/library/en-us/winsock/winsock/windows_sockets_api_reference_2.asp

template< class Ch, class Tr >
void tcp_streambuf< Ch, Tr >::WSA_ShowError( string sockcall )
{
	stringstream ss;

	int lasterr = WSAGetLastError();

	ss << "socket error on " << sockcall << " was " << lasterr;

	switch ( lasterr )
	{
		case WSAENOTCONN : ss << " WSAENOTCONN"; break;
		case WSAETIMEDOUT : ss << " WSAETIMEDOUT"; break;
		case WSAECONNRESET : ss << " WSAECONNRESET"; break;
		case WSAENETRESET : ss << " WSAENETRESET"; break;
		case WSAEADDRINUSE : ss << " WSAEADDRINUSE"; break;

		default: ss << " undecoded error" << endl;
	}
	ss << endl;
	itsSockerr = ss.str();

	was_error = true;
}

/*-----------------------------------------------------------------*/

// client_init
//
template< class Ch, class Tr >
bool tcp_streambuf< Ch, Tr >::client_init( long server_ip_add, int port )
{
	init_win_sockets();

	itsSocket = socket( AF_INET, SOCK_STREAM, 0 );
	if ( itsSocket == INVALID_SOCKET )
	{
		return( false );
	}

	sockaddr_in si;
	si.sin_addr.S_un.S_addr = htonl( server_ip_add );
	si.sin_family = AF_INET;
	si.sin_port = htons( port );

	if ( connect( itsSocket, ( SOCKADDR * ) & si, sizeof( SOCKADDR ) ) == SOCKET_ERROR )
	{
		WSA_ShowError( "connect" );
		return( false );
	}

	socket_active = true; was_error = false;
	return( true );
};

/*-----------------------------------------------------------------*/

// server_init
//
template< class Ch, class Tr >
bool tcp_streambuf< Ch, Tr >::server_init( int port )
{
	init_win_sockets();

	sockaddr_in si;
	si.sin_addr.S_un.S_addr = htonl( INADDR_ANY );
	si.sin_family = AF_INET;
	si.sin_port = htons( port );
	SOCKADDR * sa = ( SOCKADDR * ) & si;

	SOCKET listen_socket = GetListenSocket( sa, port );

	if ( listen( listen_socket, 5 ) == SOCKET_ERROR )
	{
		WSA_ShowError( "listen" );
		return( false );
	}

	cout << "wait for accept on " << MyIpAddress() << endl;

	int nLengthAddr = sizeof( SOCKADDR );
	itsSocket = accept( listen_socket, sa, & nLengthAddr );
	if ( itsSocket == INVALID_SOCKET )
	{
		WSA_ShowError( "accept" );
		return false;
	}

	socket_active = true; was_error = false;
	return( true );
};

/*-----------------------------------------------------------------*/

template< class Ch, class Tr >
SOCKET tcp_streambuf< Ch, Tr >::GetListenSocket( SOCKADDR * sa, int port )
	{
		map< int, SOCKET >::iterator it = itsListen_sockets.find( port );
		SOCKET listen_socket;

		if ( it == itsListen_sockets.end() )
		{
			listen_socket = socket( AF_INET, SOCK_STREAM, 0 );
			if ( listen_socket == INVALID_SOCKET )
			{
				WSA_ShowError( "socket" );
				return( 0 );
			}

			if ( bind( listen_socket, sa, sizeof( SOCKADDR ) ) == SOCKET_ERROR )
			{
				WSA_ShowError( "bind" );
				return( 0 );
			}

			itsListen_sockets.insert( pair< int, SOCKET >( port, listen_socket ) );
		}
		else
		{
			listen_socket = it->second;
		}

		return( listen_socket );
	}

/*-----------------------------------------------------------------*/

template< class Ch, class Tr >
char * tcp_streambuf< Ch, Tr >::MyIpAddress()
{
	char ac[80];

	if ( - 1 == gethostname( ac, sizeof( ac ) ) )
	{
		return( "gethostname failed" );
	}

	struct hostent * phe = gethostbyname( ac );
	if ( ! phe )
	{
		return( "gethostbyname failed" );
	}

	struct in_addr * addr = ( struct in_addr * ) ( phe->h_addr_list[0] );

	return ( inet_ntoa( * addr ) );
}

/*-----------------------------------------------------------------*/

// Send data out
//
template< class Ch, class Tr >
void tcp_streambuf< Ch, Tr >::dump_tcp_output()
	{
		// Output from pbase() to pptr()
		Ch * end_p = pptr();
		Ch * p = pbase();
		int size, bytes_sent;
		while ( p != end_p )
		{
			size = ( int ) ( pptr() - p );
			bytes_sent = send( itsSocket, p, size, 0 );

			if ( SOCKET_ERROR == bytes_sent )
			{
				WSA_ShowError( "send" );
			}
			else
			{
				p += bytes_sent;
				// cout << "sent " << bytes_sent << ", ";
			}
		}

		// Set pptr() to buffer start
		setp( itsBuff_out, & itsBuff_out[BUFF_SIZE] );
	};

/*-----------------------------------------------------------------*/

template< class Ch, class Tr >
	int tcp_streambuf< Ch, Tr >::sync()
	{
		// We have a partial buffer
		// to empty:  pbase() to pptr()
		dump_tcp_output();

		return( 0 );
	};

/*-----------------------------------------------------------------*/

#define FILESYS_PORT 74

void samp_tcp_stream( bool do_server )
{
	string i;

	if ( do_server )
	{
		tcp_iostream server_tcp( FILESYS_PORT );

		// Echo one string.
		server_tcp >> i;
		cout << "received " << i << endl;
		server_tcp << i << endl; // Return with white space
		cout << "sent " << i << endl;
	}
	else
	{
		tcp_iostream client_tcp( 0x7f000001, FILESYS_PORT );
		i = "HerbertHooverPresidentOfTheUnitedStates";
		client_tcp << i << endl;
		cout << "sent " << i << endl;
		client_tcp >> i;
		cout << "received " << i << endl;
	}

	// cout << "press enter to exit" << endl ;
	// cin >> i;
}
