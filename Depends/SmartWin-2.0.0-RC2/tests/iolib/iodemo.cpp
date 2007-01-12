// $Revision: 1.22 $
/* Copyright (c) 2005 - Thomas Hansen
 * Part of the SmartWin++ library example code
 * Permission to use under the same terms as the actual library
 * The license to the library can be found at http://smartwinlib.org
 * License for library is Open Source and BSD
 */
// iolib.cpp
// Show the SmartWin++ library features
// that are contained in the io subdirectory of SmartWin.
// These are:
//
#include "SmartSOAP.h"          // Fundamental include
#include "SmartWin.h"           // Fundamental include
#include "io/textbox_stream.h"  // iostream for textboxes; an easy way to port command line programs.
#include "io/iolib.h"           // Print, ftp, email, and transfer strings and files.
#include "io/html_put.h"        // Serialize widgets into a html file.
#include "io/html_get.h"        // Convert a html file subset back into widget contents.
#include "io/InDialog.h"        // Input some integers, strings, and booleans.

// The file state.html contains the starting state of the program and is worth a look.

// Standard C++ of 1998 includes
#include <iostream>
#include <fstream>
#include <sstream>
using namespace SmartWin;

class IolibClass
			: public WidgetFactory< WidgetWindow, IolibClass >
{
private:
	WidgetMenuPtr itsMainMenu, itsPopupMenu;

	WidgetCheckBoxPtr itsHTTPGetCheck, itsHTTPRespCheck, itsHtmlFileCheck;
	WidgetTextBoxPtr itsHTTPGet, itsHTTPResp, itsHtmlFile;
	WidgetComboBoxPtr itsHostname;
	WidgetSliderPtr itsHorizSlider;
	WidgetSpinnerPtr itsSpinner;
	WidgetDataGridPtr itsList;
	WidgetTreeViewPtr itsTree;

	std::iostream * itsTextboxStream;

public:
	IolibClass()
	{}

	// Ask for a html filename, and then save the various widgets to that file.
	void saveState( WidgetMenuPtr menu, unsigned item )
	{
		WidgetSaveFile dlg = createSaveFile();
		dlg.addFilter( _T( "HTML table files (*.html)" ), _T( "*.html" ) );
		dlg.addFilter( _T( "All Files (*.*)" ), _T( "*.*" ) );
		SmartUtil::tstring filePath = dlg.showDialog();
		if ( 0 != filePath.size() )
			FileSave( filePath );
	}

	// Save the various widgets to "filePath".
	void FileSave( SmartUtil::tstring & filePath )
	{
		fstream file( filePath.c_str(), ios_base::out );
		if ( ! file.good() )
			return;

		html_put hp( file ); // Constructor writes the header:  <html> <body> etc

		// Each put converts the content of the widget into html text, and adds it to the file.
		hp.put( itsHostname );

		hp.put( itsHTTPGetCheck ); hp.put( itsHTTPRespCheck ); hp.put( itsHtmlFileCheck );

		hp.put( itsHTTPGet ); hp.put( itsHTTPResp ); hp.put( itsHtmlFile );

		hp.put( itsSpinner );
		hp.put( itsHorizSlider );

		hp.put( itsList );
		hp.put( itsTree );

		// ~html_put() completes the html started by html_put():  </html> </body> etc
	}

	// Ask for a html filename,
	// restore the state of the various widgets from that file.
	// Naturally the file shhouold have been written by FileSave() above.
	//
	void loadState( WidgetMenuPtr menu, unsigned item )
	{
		WidgetLoadFile dlg = createLoadFile();
		dlg.addFilter( _T( "Text files (*.html)" ), _T( "*.html" ) );
		dlg.addFilter( _T( "All files (*.*)" ), _T( "*.*" ) );

		SmartUtil::tstring filePath = dlg.showDialog();
		if ( 0 != filePath.size() )
			loadState( filePath );
	}

	// Restore the state of the various widgets from "filePath".
	void loadState( SmartUtil::tstring filePath )
	{
		fstream file( filePath.c_str(), ios_base::in );
		if ( ! file.good() )
			return;

		bool ok;
		html_get hg( file, ok ); // Constructor parses through <form>
		if ( ! ok ) return;

		SmartUtil::tstring errTitle = _T( "HTML parse error" );

		// Host combobox.
		if ( ! hg.get( itsHostname ) )
		{
			createMessageBox().show( _T( "Tree" ), errTitle ); return;
		}

		// Checkboxes
		if ( ! hg.get( itsHTTPGetCheck ) )
		{
			createMessageBox().show( _T( "itsHTTPGet check" ), errTitle ); return;
		}
		if ( ! hg.get( itsHTTPRespCheck ) )
		{
			createMessageBox().show( _T( "itsHTTPResp check" ), errTitle ); return;
		}
		if ( ! hg.get( itsHtmlFileCheck ) )
		{
			createMessageBox().show( _T( "itsHtmlFile check" ), errTitle ); return;
		}

		// Textboxes
		if ( ! hg.get( itsHTTPGet ) )
		{
			createMessageBox().show( _T( "itsHTTPGet" ), errTitle ); return;
		}
		if ( ! hg.get( itsHTTPResp ) )
		{
			createMessageBox().show( _T( "itsHTTPResp" ), errTitle ); return;
		}
		if ( ! hg.get( itsHtmlFile ) )
		{
			createMessageBox().show( _T( "itsHtmlFile" ), errTitle ); return;
		}

		if ( ! hg.get( itsSpinner ) )
		{
			createMessageBox().show( _T( "itsSpinner" ), errTitle ); return;
		}
		if ( ! hg.get( itsHorizSlider ) )
		{
			createMessageBox().show( _T( "itsHorizSlider" ), errTitle ); return;
		}

		if ( ! hg.get( itsList ) )
		{
			createMessageBox().show( _T( "itsList" ), errTitle ); return;
		}
		if ( ! hg.get( itsTree ) )
		{
			createMessageBox().show( _T( "itsTree" ), errTitle ); return;
		}
	}

	// The iostream itsTextboxStream is used for command line   (c:/> never dies !)
	// like input and output inside a WidgetTextBox.
	//
	// First we need to create the iostream with this line:
	//   itsTextboxStream= new TextBox_iostream<IolibClass>( itsHTTPGet );
	//
	// The ability to use an iostream to write text to a TextBox lets you
	// operate at a higher level of abstraction, and reuse functions to also
	// write to files or stringstreams.
	//
	// The ability to use an iostream to accept data from the keyboard,
	// (and echo it to the TextBox) is somewhat useful for porting existing
	// "cout" and "cin" type programs to SmartWin++ without rewriting everything.
	// However, waiting for user keyboard input has the bad side effect of
	// not responding to other events until the keyboard input occurs.
	// For example, the program will wait forever until you press the "ENTER" key !
	// So, write new GUI programs with this class with a warning.
	//
	// Reading text from a TextBox is easily done in an iostream way as shown.
	//
	void menuIostreamTextBox( WidgetMenuPtr menu, unsigned item )
	{
		* itsTextboxStream << "Enter text here = "; // Arrives in WidgetTextBox itsHTTPGet
		SmartUtil::tstring textin;
		* itsTextboxStream >> textin; // gets text from the user's keyboard.
		* itsTextboxStream << _T( "You entered " ) << textin
		<< std::endl;

		// How to use iostreams to read text from the WidgetTextBox itsHTTPGet
		stringstream ss( itsHTTPGet->getText() );
		string first_token;
		ss >> first_token; // Will get "Enter".
	}

	void menuInDialog( WidgetMenuPtr menu, unsigned item )
	{
		int int1 = - 8, int2 = 0, int3 = 0;
		BoundedLong bl( 1994, 1900, 2050 );
		float float1 = ( float ) 98.6;
		double doub1 = 876.543;
		SmartUtil::tstring str1 = "hello";
		PasswordString pass1; // or ( 8, "" )
		bool bool1 = true;

		ChoiceString cs( "scissors|rock|paper", 2 );

		FileString loadFs( "C:\\path\\gumbo.txt" );
		loadFs.addFilter( "JPG files", "*.jpg" );
		FileString saveFs( "", false );
		saveFs.addFilter( "JPG files", "*.jpg" );

		SYSTEMTIME bookDue;
		::GetSystemTime( & bookDue );

		COLORREF background = RGB( 20, 40, 80 );

		bool c;

		InDialog myInDialog( this, "First InDialog Attempt !" );
		if ( IDCANCEL == myInDialog
			 .add( _T( "Input file ..." ), & loadFs )
			 .add( _T( "Output file ..." ), & saveFs )
			 .add( _T( "Temperature:" ), & int1 )
			 .add( _T( "Average temperature:" ), & float1 )
			 .add( _T( "Average weight:" ), & doub1 )
			 .add( _T( "Birth year (1900-2050)" ), & bl )
			 .add( _T( "Fries ?" ), & bool1 )
			 .add( _T( "Say:" ), & str1 )
			 .add( _T( "Password:" ), & pass1 )
			 .add( _T( "Throw:" ), & cs )
			 .add( _T( "Book due:" ), & bookDue )
			 .add( _T( "Background color ..." ), & background )
			 .createDialog() ) return;

		InDialog myInDialog2( this, "A really long title will make the dialog adjust to its width." );
		myInDialog2.add( "Pandas:", & int2 ).createDialog();

		InDialog myInDialog3( this, "Short Title." );
		myInDialog3.add( "Pandas:", & int3 ).createDialog();

		// Ask for a font for the next dialog.
		WidgetChooseFont widget = createChooseFont();
		FontPtr font = widget.showDialog();
		if ( font.get() != 0 )
		{
			FontPtr choosenFont( font );

			InDialog myInDialog5( this, "InDialog with variable fonts !", choosenFont );

			char * srpArray[] =
			{ "scissors", "rock", "paper"
			};
			vector< SmartUtil::tstring > srp( srpArray, & srpArray[ sizeof( srpArray ) / sizeof( char * ) ] );
			ChoiceString csVector( srp, 2 );

			myInDialog5
			.add( "Hippos:", & int1 )
			.add( "Ducks:", & int2 )
			.add( "Otters:", & int3 )
			.add( "Say:", & str1 )
			.add( "Password:", & pass1 )
			.add( "Throw:", & csVector )
			.add( "Average weight:", & doub1 )
			.add( "Fries ?", & bool1 )
			.createDialog();
		}
	}

	void checkclicked( WidgetCheckBoxPtr chk )
	{
		layout();
	}

	void selectionChanged( WidgetComboBoxPtr comboBox )
	{}

	void menuClearText( WidgetMenuPtr menu, unsigned item )
	{
		itsHTTPGet->setText( "" );
		itsHTTPResp->setText( "" );
		itsHtmlFile->setText( "" );
	}

	// This is a demonstration of the iostream for simple TCP/IP communication.
	// The example uses the HTTP protocol to ask a web server for a file.
	//
	// WidgetTextBox itsHTTPGet displays the HHTP message we send to the web server.
	// WidgetTextBox itsHTTPRes displays the HHTP response.
	// WidgetTextBox itsHtmlFile displays the file returned by the web server.
	//
	void menuTcpStream( WidgetMenuPtr menu, unsigned item )
	{
		string host = itsHostname->getSelectedValue();
		SmartNetwork::TcpTextStream tcp( host.c_str(), "80", 30 ); // Port 80 for a HHTP web server.
		itsHTTPGet->setText( host + _T( " connected." ) );

		stringstream ss;
		ss << "GET / HTTP/1.1\r\n"
		<< "Accept: */*\r\n"
		<< "Host: " << host << "\r\n"
		<< "Connection: Keep-Alive\r\n"
		<< "\r\n";
		tcp << ss.str();
		itsHTTPGet->setText( ss.str() );

		string headers, content;
		fetchHeadersContent( tcp, headers, content );
		itsHTTPResp->setText( headers );    // The response from the webserver
		itsHtmlFile->setText( content );    // The file returned by the webserver.
	}

	void menuTcpStream2( WidgetMenuPtr menu, unsigned item )
	{
		string  host = itsHostname->getSelectedValue();
		SmartNetwork::TcpTextStream tcp( host.c_str(), "80", 30 );  // Port 80 for a HHTP web server.

		tcp << itsHTTPGet->getText();

		string headers, content;
		fetchHeadersContent( tcp, headers, content );
		itsHTTPResp->setText( headers );    // The response from the webserver
		itsHtmlFile->setText( content );    // The file returned by the webserver.
	}

	void fetchHeaders( SmartNetwork::TcpTextStream & tcp, string & headers )
	{
		const int bsize = 99999;
		char buff[ bsize ];
		while ( tcp.getline( buff, bsize ) )
		{
			int n = tcp.gcount();
			if ( n < 3 ) break;
			headers += buff;
			headers += "\n";
		}
	}

	// Rather uninteresting code unless you are interested in HTTP
	void fetchHeadersContent( SmartNetwork::TcpTextStream & tcp, string & headers, string & content )
	{
		long length = 0;
		bool length_known = false;

		stringstream ss;
		string name;
		const int bsize = 65536;
		char buff[ bsize ];
		while ( tcp.getline( buff, bsize ) )
		{
			headers += buff;
			headers += "\n";

			ss.str( buff );
			if ( ss.str().size() < 3 )
			{
				content.clear();

				const int chunksize = 4096;
				char buff[ chunksize ];
				streamsize chunk = chunksize;
				if ( length_known )
				{
					while ( length > 0 )
					{
						if ( length < chunksize ) chunk = length;
						tcp.read( buff, chunk );
						length -= chunk;
						content.append( buff, chunk );
					}
				}
				else
				{
					while ( tcp.getline( buff, bsize ) )
					{
						content.append( buff );
						content.append( "\r\n" );
					}
				}
				return ;
			}

			ss >> name;
			if ( name == "Content-Length:" )
			{
				ss >> length;
				length_known = true;
			}
		}
	}

	void menuCloseEvent( WidgetMenuPtr menu, unsigned item )
	{
		close();    // Exit the window.  All Widgets are automatically removed.
	}

	void copyTextToClipboard( WidgetMenuPtr menu, unsigned item )
	{
		SmartUtil::tstring selection = itsHTTPGet->getSelection();
		ClipBoardString::instance().setClipBoardData( selection, this );
	}

	void initAndCreate()
	{
		createWindow();
		setText( _T( "Iolib demonstration" ) );

		SmartWin::Rectangle desktop( getDesktopSize() );
		setBounds( desktop.top().right() );

		// Creating Widgets
		WidgetTextBox::Seed editSeed;
		editSeed.style |= ES_MULTILINE | ES_WANTRETURN | ES_AUTOVSCROLL;

		itsHTTPGet = createTextBox( editSeed );
		itsHTTPResp = createTextBox( editSeed );
		itsHtmlFile = createTextBox( editSeed );

		itsTextboxStream = new TextBox_iostream<IolibClass>( itsHTTPGet );

		itsHostname = createComboBox();
		itsHostname->onSelectionChanged( &IolibClass::selectionChanged );
		itsHostname->setBounds( SmartWin::Rectangle( Point( 200, 100 ) ) );

		itsHTTPGetCheck = createCheckBox(); itsHTTPGetCheck->setChecked( true );
		itsHTTPRespCheck = createCheckBox();    itsHTTPRespCheck->setChecked( true );
		itsHtmlFileCheck = createCheckBox();    itsHtmlFileCheck->setChecked( true );

		SmartWin::Rectangle chksize = SmartWin::Rectangle( Point( 100, 30 ) );
		itsHTTPGetCheck->setBounds( chksize );
		itsHTTPRespCheck->setBounds( chksize );
		itsHtmlFileCheck->setBounds( chksize );

		itsHTTPGetCheck->onClicked( &IolibClass::checkclicked );
		itsHTTPRespCheck->onClicked( &IolibClass::checkclicked );
		itsHtmlFileCheck->onClicked( &IolibClass::checkclicked );

		// The rest of the Widgets are just for saving and restoring their state.
		itsSpinner = createSpinner();
		itsSpinner->setRange( 0, 10 );
		itsSpinner->setValue( 10 );
		itsSpinner->setBounds( SmartWin::Rectangle( Point( 100, 30 ) ) );

		itsHorizSlider = createSlider();
		itsHorizSlider->setAutoTicks( true );
		itsHorizSlider->setHorizontal( true );
		itsHorizSlider->setShowTicksBoth( true );
		itsHorizSlider->setRange( 0, 100 );
		itsHorizSlider->setPosition( 100 );
		itsHorizSlider->setTickFrequency( 10 );
		itsHorizSlider->setBounds( SmartWin::Rectangle( Point( 100, 30 ) ) );

		itsList = createDataGrid();
		itsTree = createTreeView();

		// Creating main menu
		itsMainMenu = createMenu();
		WidgetMenuPtr file = itsMainMenu->appendPopup( _T( "&File" ) );
		int m = 1;
		file->appendItem( m++, _T( "iostream in TextBox" ), &IolibClass::menuIostreamTextBox );
		file->appendSeparatorItem();
		file->appendItem( m++, _T( "Try InDialog" ), &IolibClass::menuInDialog );
		file->appendSeparatorItem();
		file->appendItem( m++, _T( "Send HTTP GET using TCP iostream" ), &IolibClass::menuTcpStream );
		file->appendItem( m++, _T( "Send the stuff in the left textbox to the host in the dropdown" ), &IolibClass::menuTcpStream2 );
		file->appendSeparatorItem();
		file->appendItem( m++, _T( "&Copy text selection" ), &IolibClass::copyTextToClipboard );
		file->appendItem( m++, _T( "&Clear text" ), &IolibClass::menuClearText );
		file->appendSeparatorItem();
		file->appendItem( m++, _T( "&Save state" ), &IolibClass::saveState );
		file->appendItem( m++, _T( "&Load state" ), &IolibClass::loadState );
		file->appendSeparatorItem();
		file->appendItem( m++, _T( "&Exit" ), &IolibClass::menuCloseEvent );
		itsMainMenu->attach( this );    // Attaching main menu to main window

		loadState( "state.html" );

		itsHostname->addValue(_T("localhost"));

		layout(); // for the first time, and only AFTER all Widgets are created.
		onSized( &IolibClass::isResized );
	}

	void isResized( const WidgetSizedEventResult & sz )
	{
		layout(); // to reflect the change in window size by the user.
		updateWidget();
	}

	// Common routine to do layout the first time, and after each resize event.
	// We divide the available space into:   Tabbed Area | Graphics Area
	//
	void layout()
	{
		SmartWin::Rectangle r( getClientAreaSize() );
		Place p;
		p.setBoundsBorders( r, 4, 4 );   // X, Y border between Widgets

		itsHostname->setPositionPerPlace( p );

		itsHTTPGetCheck->setPositionPerPlace( p );
		itsHTTPRespCheck->setPositionPerPlace( p );
		itsHtmlFileCheck->setPositionPerPlace( p );

		r = r.upperLeftAdjust( Point( 0, itsHtmlFileCheck->getSize().y ) );
		p.setBoundsBorders( r, 4, 4 );   // X, Y border between Widgets

		int cols = 0;
		if ( itsHTTPGetCheck->getChecked() ) cols++;
		if ( itsHTTPRespCheck->getChecked() ) cols++;
		if ( itsHtmlFileCheck->getChecked() ) cols++;

		if ( itsHTTPGetCheck->getChecked() ) itsHTTPGet->setSizeAsGridPerPlace( p, 2, cols );
		itsHTTPGet->setVisible( itsHTTPGetCheck->getChecked() );

		if ( itsHTTPRespCheck->getChecked() ) itsHTTPResp->setSizeAsGridPerPlace( p, 2, cols );
		itsHTTPResp->setVisible( itsHTTPRespCheck->getChecked() );

		if ( itsHtmlFileCheck->getChecked() ) itsHtmlFile->setSizeAsGridPerPlace( p, 2, cols );
		itsHtmlFile->setVisible( itsHtmlFileCheck->getChecked() );

		int row = 3;
		cols = 4;
		itsSpinner->setSizeAsGridPerPlace( p, row, cols );
		itsHorizSlider->setSizeAsGridPerPlace( p, row, cols );

		itsList->setSizeAsGridPerPlace( p, row, cols );
		itsTree->setSizeAsGridPerPlace( p, row, cols );
	}
};

// The main creates an instance of the class and then starts the SmartWin application.
//
int SmartWinMain( Application & app )
{
	IolibClass * testHello = new IolibClass;
	testHello->initAndCreate();
	return app.run();
}

/*

APACHE WEB SERVER RESPONSE:
HTTP/1.1 200 OKDate: Mon, 04 Jul 2005 16:20:01 GMTServer: Apache/1.3.31 (Win32)Content-Location: index.html.enVary: negotiate,accept-language,accept-charsetTCN: choiceLast-Modified: Thu, 03 May 2001 21:00:38 GMTETag: "0-5d6-3af1c6f6;40d03c77"Accept-Ranges: bytesContent-Length: 1494Keep-Alive: timeout=15, max=100Connection: Keep-AliveContent-Type: text/htmlContent-Language: en_________________________________________________________Webserver on DELLHTTP/1.1 200 OKVia: 1.1 IRVPROXY01Content-Length: 21263Expires: Thu, 01 Dec 1994 8:00:00 GMTDate: Mon, 04 Jul 2005 16:23:20 GMTContent-Location: http://www.dell.com/default.htmContent-Type: text/htmlLast-Modified: Mon, 04 Jul 2005 11:55:27 GMTAccept-Ranges: bytesETag: "b08de1448f80c51:60d"P3P: CP="BUS CAO CNT COM CUR DEV DSP INT NAV OUR PSA PSD SAM STA TAI UNI"P3P: policyref="http://www.dell.com/w3c/p3p.xml", CP="BUS CAO CNT COM CUR DEV DSP INT NAV OUR PSA PSD SAM STA TAI UNI"Set-Cookie: SITESERVER=ID=20c3c5359fdd4ca8b328bb3d662f3c44; domain=.dell.com; path=/; expires=Sun, 04-Jul-2010 16:23:20 GMT;Set-Cookie: SITESERVER_SESSION=ID=20c3c5359fdd4ca8b328bb3d662f3c44; domain=.dell.com; path=/;Server: Unauthorized-Use-Prohibited

_________________________________________________________
Webserver used on Google

HTTP/1.1 200 OKVia: 1.0 IRVPROXY01Connection: closeProxy-Connection: closeDate: Mon, 04 Jul 2005 16:24:52 GMTContent-Type: text/htmlCache-Control: privateSet-Cookie: PREF=ID=014f4c9453996089:TM=1120494292:LM=1120494292:S=UjK9Z6pD31up-saw; expires=Sun, 17-Jan-2038 19:14:07 GMT; path=/; domain=.google.comServer: GWS/2.1

Note no Content-Length !

_________________________________________________________
Webserver used on IBM

HTTP/1.1 302 FoundVia: 1.1 IRVPROXY01Connection: closeProxy-Connection: closeDate: Mon, 04 Jul 2005 16:33:16 GMTLocation: http://www.ibm.com/us/Content-Type: text/htmlServer: IBM_HTTP_SERVER
_________________________________________________________
Webserver for Sun

HTTP/1.1 200 OKVia: 1.1 IRVPROXY01Connection: closeProxy-Connection: closeDate: Mon, 04 Jul 2005 16:34:17 GMTContent-Type: text/html; charset=UTF-8Server: Sun Java System Web Server 6.1P3p: policyref="http://www.sun.com/p3p/Sun_P3P_Policy.xml", CP="CAO DSP COR CUR ADMa DEVa TAIa PSAa PSDa CONi TELi OUR  SAMi PUBi IND PHY ONL PUR COM NAV INT DEM CNT STA POL PRE GOV"Set-Cookie: SUN_ID=198.17.235.19:5721120494857; EXPIRES=Wednesday, 31-Dec-2025 23:59:59 GMT; DOMAIN=.sun.com; PATH=/Set-Cookie: JSESSIONID=E7DDE26FAA66316BC46922EDB54F8307.tomcat2;Path=/
_________________________________________________________
Webserver for Apple

HTTP/1.1 200 OKVia: 1.0 IRVPROXY01Content-Length: 12671Age: 121Expires: Mon, 04 Jul 2005 16:37:59 GMTDate: Mon, 04 Jul 2005 16:27:59 GMTContent-Type: text/htmlCache-Control: max-age=600Server: Apache/1.3.29 (Darwin) PHP/4.3.1

*/
