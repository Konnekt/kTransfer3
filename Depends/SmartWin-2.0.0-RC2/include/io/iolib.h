// $Revision: 1.10 $
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
#ifndef iolib_8767862393
#define iolib_8767862393

#include "SmartWin.h"
using namespace SmartWin;

#include <fstream>
#include <string>
#include <sstream>
#include <queue>
#include <wininet.h>
#include <mapi.h>
using namespace std;

//-------------------------------------------------------------------------

class iolib
{
public:

void StringToBytes( const string & hexstring, unsigned char dat[], int & len )
{
	len = 0;

	stringstream hexstr( hexstring );
	hexstr << hex;
	int b;
	while ( hexstr >> b ) dat[len++] = b; // dat has the data now.
}

//---------------------

void BytesToString( unsigned char dat[], int len, string & hexstring )
{
	stringstream out_hash;
	out_hash << hex;

	int n = 0;
	unsigned int b;
	for ( int i = 0; i < len; i++ )
	{
		b = dat[i];
		if ( b < 0x10 ) out_hash << '0';
		out_hash << b; // Integer converted to hex string
		if ( ++n == 16 )
		{
			out_hash << "\r\n";
			n = 0;
		}
		else
		{
			out_hash << " ";
		}
	}

	hexstring = out_hash.str();
}

//-------------------------------------------------------------------------

bool FileToString( SmartUtil::tstring & filePath, SmartUtil::tstring & filedata, bool hex_editing )
{
	if ( 0 == filePath.size() ) return( false );

	if ( hex_editing )
	{
		// Read in the binary file a byte at a time.
		basic_ifstream< TCHAR > file( SmartUtil::AsciiGuaranteed::doConvert( filePath.c_str(), SmartUtil::ConversionCodepage::UTF8 ).c_str(), ios_base::binary );
		if ( !file.good() ) return( false );

		const int bytes_per_line = 24;
		int n = 0;
		stringstream hexstr;
		hexstr << hex;
		int b = file.get();
		while ( file.good() )
		{
			hexstr << b;

			if ( ++n == bytes_per_line )
			{
				hexstr << "\r\n";
				n = 0;
			}
			else
			{
				hexstr << " ";
			}
			b = file.get();
		}
		filedata = hexstr.str();
	}
	else
	{
		// Read in the ASCII file a line at a time.
		basic_ifstream< TCHAR > file( SmartUtil::AsciiGuaranteed::doConvert( filePath.c_str(), SmartUtil::ConversionCodepage::UTF8 ).c_str() );
		if ( !file.good() ) return( false );

		const int bsize = 65536;
		char buff[bsize];
		while ( file.getline( buff, bsize ) )
		{
			filedata += buff;
			filedata += "\r\n";
		}
	}

	return( true );
}

//-------------------------------------------------------------------------

bool StringToFile( const SmartUtil::tstring & filePath, const SmartUtil::tstring & filedata, bool hex_editing )
{
	if ( 0 == filePath.size() != 0 ) return( false );


	if ( hex_editing )
	{
		basic_ofstream< TCHAR > file( SmartUtil::AsciiGuaranteed::doConvert( filePath.c_str(),
									  SmartUtil::ConversionCodepage::UTF8 ).c_str(), ios_base::binary );
		if ( ! file.good() ) return( false );

		// Parse the "f3 32 85 d2" data into the corresponding characters.
		stringstream hexstr( filedata );
		hexstr << hex;

		int b;
		while ( hexstr >> b )
		{
			file.put( b );
		}
	}
	else
	{
		basic_ofstream< TCHAR > file( SmartUtil::AsciiGuaranteed::doConvert( filePath.c_str(),
								  SmartUtil::ConversionCodepage::UTF8 ).c_str() );
		if ( ! file.good() ) return( false );

		// Put the text into the file as it is.
		file << filedata;
	}

	return( true );
}

//-------------------------------------------------------------------------

static bool print( string & printtext )
{
	PRINTDLG pd;

	memset( & pd, 0, sizeof( pd ) );

	pd.lStructSize = sizeof( pd );

	/*
	 * get rid of PD_RETURNDEFAULT on the line below if you'd like to
	 * see the "Printer Settings" dialog!
	 *
	 */
	pd.Flags = PD_RETURNDEFAULT | PD_RETURNDC;

	// try to retrieve the printer DC
	if ( !PrintDlg( & pd ) )
	{
		MessageBox( NULL, "PrintDlg( &pd ) failed!", "Fatal Error", MB_OK | MB_ICONERROR );

		return false;
	}

	DOCINFO di;
	HDC hPrinter = pd.hDC;

	// initialization of the printing!
	memset( & di, 0, sizeof( di ) );
	di.cbSize = sizeof( di );
	StartDoc( hPrinter, & di );

	TEXTMETRIC Metrics;
	GetTextMetrics( hPrinter, & Metrics );

	int page_width = GetDeviceCaps( hPrinter, HORZRES ); // 5100
	int page_height = GetDeviceCaps( hPrinter, VERTRES ); // 6600
	const int linesize = 999;
	char line[linesize];

	stringstream doc( printtext );

	// uncomment the following line to print in colour! :)
	// SetTextColor( hPrinter, 0x0000FF );

	int len, y_pos = 0;
	StartPage( hPrinter );

	while ( doc.getline( line, linesize ) )
	{
		len = ( int ) strlen( line );
		if ( line[len - 1] == '\n' ) len--;
		TextOut( hPrinter, 100, y_pos, line, len );

		y_pos += Metrics.tmHeight;
		if ( y_pos > page_height )
		{
			y_pos = 0;
			EndPage( hPrinter );
			StartPage( hPrinter );
		}
	}

	// end this document and release the printer's DC
	EndDoc( hPrinter );
	DeleteDC( hPrinter );
	return true;
}

//-------------------------------------------------------------------------

static bool FtpWriteFile( string & ftptext, string & server, string fname,
						string & username, string & password )
{
	HINTERNET io_hand = InternetOpen( "Me", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0 );

	DWORD dwContext = 123;
	HINTERNET ic_hand = InternetConnect( io_hand, server.c_str(), INTERNET_DEFAULT_FTP_PORT, username.c_str(), password.c_str(),
										INTERNET_SERVICE_FTP, INTERNET_FLAG_PASSIVE, ( DWORD_PTR ) & dwContext );
	if ( NULL == ic_hand )
	{
		InternetCloseHandle( io_hand ); return( false );
	}

	bool ok;
	HINTERNET f_hand = FtpOpenFile( ic_hand, fname.c_str(), GENERIC_WRITE,
									FTP_TRANSFER_TYPE_BINARY, ( DWORD_PTR ) & dwContext );

	DWORD BytesWritten;
	ok = TRUE == InternetWriteFile( f_hand, ftptext.c_str(), ( DWORD ) ftptext.length(), & BytesWritten );

	InternetCloseHandle( f_hand );

	InternetCloseHandle( ic_hand );
	InternetCloseHandle( io_hand );

	return( ok );
}

//-------------------------------------------------------------------------

static bool FtpReadFile( string & server, string fname, string & username, string & password,
						 string & ftptext )
{
	HINTERNET io_hand = InternetOpen( "Me", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0 );

	DWORD dwContext = 123;
	HINTERNET ic_hand = InternetConnect( io_hand, server.c_str(), INTERNET_DEFAULT_FTP_PORT, username.c_str(), password.c_str(),
										INTERNET_SERVICE_FTP, INTERNET_FLAG_PASSIVE, ( DWORD_PTR ) & dwContext );
	if ( NULL == ic_hand )
	{
		InternetCloseHandle( io_hand ); return( false );
	}

	HINTERNET f_hand = FtpOpenFile( ic_hand, fname.c_str(), GENERIC_READ,
									FTP_TRANSFER_TYPE_BINARY, ( DWORD_PTR ) & dwContext );

	// Read in the FTP file a chunk at a time.
	ftptext = "";
	BOOL ReadOk;
	DWORD BytesRead = 0;
	const int bsize = 65536;
	char buff[bsize];
	while ( ReadOk = InternetReadFile( f_hand, buff, bsize, & BytesRead ) )
	{
		if ( 0 == BytesRead ) break;
		ftptext.append( buff, BytesRead );
	}

	InternetCloseHandle( f_hand );

	InternetCloseHandle( ic_hand );
	InternetCloseHandle( io_hand );

	return( TRUE == ReadOk );
}

//-------------------------------------------------------------------------

static bool FileEmailFile( string & emailtext )
{
#ifndef __WINE__
	// Get address of function from MAPI32.DLL
	HINSTANCE hDLL = LoadLibrary( "mapi32" ); if ( NULL == hDLL ) return( false );
	LPMAPISENDMAIL SendMail = ( LPMAPISENDMAIL ) GetProcAddress( hDLL, "MAPISendMail" );
	if ( NULL == SendMail )
	{
		FreeLibrary( hDLL );
		return( false );
	}

	// Send a mail message containing a file and prompt for
	// recipients, subject, and note text.

	MapiFileDesc attachment =
	{
			0,         // ulReserved, must be 0
			0,         // no flags; this is a data file
			( ULONG ) - 1, // position not specified
			"c:\\vpnotes.txt",  // pathname
			"vpnotes.txt",      // original filename
			NULL
			}; // MapiFileTagExt unused

	// Create a blank message. Most members are set to NULL or 0 because
	// MAPISendMail will let the user set them.

	MapiMessage note =
	{ 0, // reserved, must be 0
							NULL, // no subject
	  ( char * ) emailtext.c_str(), // NULL = no note text
					NULL,         // NULL = interpersonal message
					NULL,         // no date; MAPISendMail ignores it
					NULL,         // no conversation ID
					0L,           // no flags, MAPISendMail ignores it
					NULL,         // no originator, this is ignored too
					0,            // zero recipients
					NULL,         // NULL recipient array
					0, // 1,            // one attachment
					& attachment
					}; // the attachment structure

	// Next, the client calls the MAPISendMail function and stores the return status so it can detect whether the call succeeded. You should use a more sophisticated error reporting mechanism than the C library function printf.

	ULONG err = SendMail ( 0L,          // use implicit session.
					0L,          // ulUIParam; 0 is always valid
					& note,       // the message being sent
					MAPI_DIALOG, // allow the user to edit the message
					0L ); // reserved; must be 0

	if ( err != SUCCESS_SUCCESS ) return( false );

	FreeLibrary( hDLL );

#endif
	return( true );
}

//-------------------------------------------------------------------------
};

#endif
