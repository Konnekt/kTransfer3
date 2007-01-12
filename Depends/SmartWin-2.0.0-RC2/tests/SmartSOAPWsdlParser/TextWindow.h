// $Revision: 1.4 $
/* Copyright (c) 2005 - Thomas Hansen
 * Part of the SmartWin++ library example code
 * Permission to use under the same terms as the actual library
 * The license to the library can be found at http://smartwinlib.org
 * License for library is Open Source and BSD
 */
#ifndef TextWindow_H
#define TextWindow_H

#include "SmartSOAP.h"
#include "SmartWin.h"
#include "WsdlParser.h"

using namespace SmartWin;
using namespace SmartUtil;
using namespace SmartXML;
using namespace SmartHTTP;

class TextWindow
	: public WidgetFactory< WidgetWindow, TextWindow >
{
	WidgetRichTextBoxPtr textWindow;

	void layout()
	{
		sw::Rectangle rc( this->getClientAreaSize() );
		textWindow->setBounds( rc.shrink( 2l ) );
	}

	void sized( const WidgetSizedEventResult & sz )
	{
		layout();
	}

	void setTextColor( long start, long end, COLORREF color )
	{
		CHARFORMAT cf;
		cf.cbSize = sizeof( CHARFORMAT );
		cf.dwMask = CFM_COLOR;
		cf.crTextColor = color;
		cf.dwEffects = 0;

		WPARAM wPar = SCF_SELECTION;
		LPARAM lPar = ( LPARAM ) & cf;
		textWindow->setSelection( start, end );
		textWindow->sendWidgetMessage( textWindow->handle(), EM_SETCHARFORMAT, wPar, lPar );
	}

	void setBold( long start, long end )
	{
		CHARFORMAT cf;
		cf.cbSize = sizeof( CHARFORMAT );
		cf.dwMask = CFM_BOLD;
		cf.dwEffects = CFE_BOLD;

		WPARAM wPar = SCF_SELECTION;
		LPARAM lPar = ( LPARAM ) & cf;
		textWindow->setSelection( start, end );
		textWindow->sendWidgetMessage( textWindow->handle(), EM_SETCHARFORMAT, wPar, lPar );
	}

	void colorizeCode( long start, long end )
	{
		// Doing a hide selection
		WPARAM wPar = 1;
		LPARAM lPar = 0;
		textWindow->sendWidgetMessage( textWindow->handle(), EM_HIDESELECTION, wPar, lPar );

		const COLORREF TEXT_COLOR = RGB( 0, 180, 30 );
		const COLORREF COMMENT_COLOR = RGB( 0, 180, 150 );
		const COLORREF KEYWORD_COLOR = RGB( 220, 30, 30 );

		// First making all text default to black
		setTextColor( 0, - 1, RGB( 0, 0, 0 ) );

		// Now making the syntax highlightning of individual words...
		tstring txt = textWindow->getText();
		long idx = 0;
		long wordStart = 0;
		for ( tstring::const_iterator iter = txt.begin();
			iter != txt.end();
			++iter )
		{
			// Found text span
			if ( * iter == _T( '"' ) )
			{
				// We've found out where to start from
				wordStart = idx;

				// "Jumping" the initial '"'...
				++iter;
				++idx;

				// Finding end of text span
				while ( iter != txt.end() && * iter != _T( '"' ) && iter != txt.begin() && * ( iter - 1 ) != _T( '\\' ) )
				{
					++iter;
					if ( * iter != _T( '\r' ) )
						++idx;
				}
				setTextColor( wordStart, idx + 1, TEXT_COLOR );
			}
			// Found text span
			if ( * iter == _T( '\'' ) )
			{
				// We've found out where to start from
				wordStart = idx;

				// "Jumping" the initial '"'...
				++iter;
				++idx;

				// Finding end of text span
				while ( iter != txt.end() && * iter != _T( '\'' ) && iter != txt.begin() && * ( iter - 1 ) != _T( '\\' ) )
				{
					++iter;
					if ( * iter != _T( '\r' ) )
						++idx;
				}
				setTextColor( wordStart, idx + 1, TEXT_COLOR );
			}
			// Found COMMENT span
			if ( * iter == _T( '/' ) && iter + 1 != txt.end() && * ( iter + 1 ) == _T( '/' ) )
			{
				// We've found out where to start from
				wordStart = idx;

				// Finding end of text span
				while ( iter != txt.end() && * iter != _T( '\n' ) )
				{
					++iter;
					if ( * iter != _T( '\r' ) )
						++idx;
				}
				setTextColor( wordStart, idx + 1, COMMENT_COLOR );
			}
			// Looking for special characters
			if ( * iter == _T( '(' ) || * iter == _T( ')' ) || * iter == _T( '{' )
				|| * iter == _T( '}' ) || * iter == _T( '[' ) || * iter == _T( ']' )
				|| * iter == _T( '<' ) || * iter == _T( '>' ) || * iter == _T( ';' )
				)
			{
				setBold( idx, idx + 1 );
			}
			// Searching for keywords...
			if ( * iter == _T( '\t' ) || * iter == _T( '\n' ) || * iter == _T( ' ' )
				|| * iter == _T( '{' ) || * iter == _T( '}' ) || * iter == _T( '(' )
				|| * iter == _T( ')' ) || * iter == _T( ';' )
				)
			{
				long tmpStart = idx;
				tstring::const_iterator startIter = iter;
				while ( startIter != txt.end() && ( * startIter == _T( '\t' ) || * startIter == _T( '\n' ) || * startIter == _T( ' ' ) ) )
					++startIter, ++tmpStart;
				if ( startIter != txt.end() )
				{
					long tmpEnd = tmpStart;
					tstring::const_iterator endIter = startIter;
					while ( endIter != txt.end() &&
						( * endIter != _T( '\t' ) &&
							* endIter != _T( '\n' ) &&
							* endIter != _T( ' ' ) ) &&
							* endIter != _T( '"' ) &&
							* endIter != _T( '(' ) &&
							* endIter != _T( ':' ) &&
							* endIter != _T( '[' )
						)
						++endIter, ++tmpEnd;

					tstring keyword( startIter, endIter );
					if ( keyword == _T( "if" ) ||
						keyword == _T( "for" ) ||
						keyword == _T( "while" ) ||
						keyword == _T( "do" ) ||
						keyword == _T( "else" ) ||
						keyword == _T( "struct" ) ||
						keyword == _T( "enum" ) ||
						keyword == _T( "switch" ) ||
						keyword == _T( "explicit" ) ||
						keyword == _T( "try" ) ||
						keyword == _T( "catch" ) ||
						keyword == _T( "throw" ) ||
						keyword == _T( "new" ) ||
						keyword == _T( "float" ) ||
						keyword == _T( "int" ) ||
						keyword == _T( "double" ) ||
						keyword == _T( "long" ) ||
						keyword == _T( "unsigned" ) ||
						keyword == _T( "delete" ) ||
						keyword == _T( "private" ) ||
						keyword == _T( "public" ) ||
						keyword == _T( "protected" ) ||
						keyword == _T( "class" ) ||
						keyword == _T( "continue" ) ||
						keyword == _T( "default" ) ||
						keyword == _T( "return" ) ||
						keyword == _T( "sizeof" ) ||
						keyword == _T( "break" ) ||
						keyword == _T( "using" ) ||
						keyword == _T( "void" ) ||
						keyword == _T( "static" ) ||
						keyword == _T( "#include" ) ||
						keyword == _T( "#ifdef" ) ||
						keyword == _T( "#ifndef" ) ||
						keyword == _T( "#else" ) ||
						keyword == _T( "static_cast" ) ||
						keyword == _T( "dynamic_cast" ) ||
						keyword == _T( "reinterpret_cast" ) )
					{
						setTextColor( tmpStart, tmpEnd, KEYWORD_COLOR );
					}
				}
			}
			if ( * iter != _T( '\r' ) )
				++idx;
		}

		// Doing a show selection
		wPar = 0;
		lPar = 0;
		textWindow->sendWidgetMessage( textWindow->handle(), EM_HIDESELECTION, wPar, lPar );
	}

	void colorizeXml( long start, long end )
	{
		// Doing a hide selection
		WPARAM wPar = 1;
		LPARAM lPar = 0;
		textWindow->sendWidgetMessage( textWindow->handle(), EM_HIDESELECTION, wPar, lPar );

		const COLORREF TEXT_COLOR = RGB( 0, 180, 30 );
		const COLORREF KEYWORD_COLOR = RGB( 220, 30, 30 );

		// First making all text default to black
		setTextColor( 0, - 1, RGB( 0, 0, 0 ) );

		// Now making the syntax highlightning of individual words...
		tstring txt = textWindow->getText();
		long idx = 0;
		long wordStart = 0;
		for ( tstring::const_iterator iter = txt.begin();
			iter != txt.end();
			++iter )
		{
			// Found text span
			if ( * iter == _T( '"' ) )
			{
				// We've found out where to start from
				wordStart = idx;

				// "Jumping" the initial '"'...
				++iter;
				++idx;

				// Finding end of text span
				while ( iter != txt.end() && * iter != _T( '"' ) && iter != txt.begin() && * ( iter - 1 ) != _T( '\\' ) )
				{
					++iter;
					if ( * iter != _T( '\r' ) )
						++idx;
				}
				setTextColor( wordStart, idx + 1, TEXT_COLOR );
			}
			// Found text span
			if ( * iter == _T( '\'' ) )
			{
				// We've found out where to start from
				wordStart = idx;

				// "Jumping" the initial '"'...
				++iter;
				++idx;

				// Finding end of text span
				while ( iter != txt.end() && * iter != _T( '\'' ) && iter != txt.begin() && * ( iter - 1 ) != _T( '\\' ) )
				{
					++iter;
					if ( * iter != _T( '\r' ) )
						++idx;
				}
				setTextColor( wordStart, idx + 1, TEXT_COLOR );
			}
			if ( * iter != _T( '\r' ) )
				++idx;
		}

		// Doing a show selection
		wPar = 0;
		lPar = 0;
		textWindow->sendWidgetMessage( textWindow->handle(), EM_HIDESELECTION, wPar, lPar );
	}

public:
	void init( const tstring & caption, const tstring & txt, COLORREF color, bool doColorize )
	{
		// Creating main window
		WidgetWindow::Seed mainSeed;
		mainSeed.caption = caption;
		createWindow( mainSeed );
		onSized( & TextWindow::sized );

		// Creating Text Window
		WidgetRichTextBox::Seed richTextBoxCS;
		richTextBoxCS.style = WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL | ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_MULTILINE | ES_WANTRETURN;
		richTextBoxCS.exStyle = WS_EX_CLIENTEDGE;
		richTextBoxCS.font = SmartWin::createFont( _T( "Courier New" ), 16, 8 );
		richTextBoxCS.backgroundColor = color;
		richTextBoxCS.location = SmartWin::Rectangle( 100, 100, 300, 300 );
		richTextBoxCS.scrollBarHorizontallyFlag = false;
		richTextBoxCS.scrollBarVerticallyFlag = false;
		textWindow = createRichTextBox( richTextBoxCS );

		// We need to increase the maximum number of characters the Rich Edit control can handle ince the default is 32767 characters!
		WPARAM wPar = 0;
		LPARAM lPar = 1000000;
		textWindow->sendWidgetMessage( textWindow->handle(), EM_EXLIMITTEXT, wPar, lPar );

		// Making TABS look beatifule...
		PARAFORMAT par;
		par.cbSize = sizeof( PARAFORMAT );
		par.dwMask = PFM_TABSTOPS;
		par.cTabCount = MAX_TAB_STOPS;
		for ( int tabPos = 0; tabPos < MAX_TAB_STOPS; ++tabPos )
		{
			if ( doColorize )
				par.rgxTabs[tabPos] = 400 * tabPos;
			else
				par.rgxTabs[tabPos] = 200 * tabPos; // Xml is only half indented
		}
		wPar = 0;
		lPar = ( LPARAM ) & par;
		textWindow->sendWidgetMessage( textWindow->handle(), EM_SETPARAFORMAT, wPar, lPar );

		// Now setting text
		textWindow->setText( txt );

		// To get initial sizig correct!!
		layout();

		if ( doColorize )
		{
			colorizeCode( 0, - 1 );
		}
		else
		{
			colorizeXml( 0, - 1 );
		}
	}
};

#endif
