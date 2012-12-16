// $Revision: 1.11 $
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
/*
Generates a subset of HTML from SmartWin Widgets:
( The routines in html_get.h parse the html generated by html_put.h. )

	html_put    hp;

	void put( iostream &io, WidgetDataGrid * grid )
	void put( iostream &io, WidgetTreeView * tree )
	void put( iostream &io, SmartUtil::tstring txt )

It uses HTML 3.2, as defined in "HTML 3.2 Reference Specification" by W3C
*/

#ifndef html_put_945746
#define html_put_945746

#include "SmartWin.h"
using namespace SmartWin;
using namespace std;

namespace SmartWin
{

class html_put
{
private:
	SmartUtil::tiostream & itsIo;

public:
	html_put( SmartUtil::tiostream & io )
		: itsIo( io )
	{
		itsIo << _T("<html> <head> <title> </title> </head>\n<body> <form>\n");
	}

	//-------------------------------------------------------------------------

	~html_put()
	{
		itsIo << _T("\n</form> </body>\n</html>\n");
	}

	//-------------------------------------------------------------------------

	// put( SmartUtil::tstring txt )
	// limitation: If the text has HTML formatting embedded, then the get will fail.
	//
	void string_put( SmartUtil::tstring txt )
	{
		itsIo << _T("<p>") << endl << txt << endl << _T("</p>") << endl;
	}

	//-------------------------------------------------------------------------

	// put( WidgetTextBox * inTextBox )
	//
	template< class Parent, class MessageMapPolicy >
	void put( SmartWin::WidgetTextBox< Parent, MessageMapPolicy > * inTextBox )
	{
		string_put( inTextBox->getText() );
	}

	//-------------------------------------------------------------------------

	// put( WidgetRichTextBox * inRichTextBox )
	//
	template< class Parent, class MessageMapPolicy >
	void put( SmartWin::WidgetRichTextBox< Parent, MessageMapPolicy, sw::RichTextBox< Parent, MessageMapPolicy > > * inRichTextBox )
	{
		string_put( inRichTextBox->getText() );
	}

	//-------------------------------------------------------------------------

	// put
	// <input type="text" name="tag" value="taggedValue" >
	//
	void putTaggedText( const SmartUtil::tstring & tag, SmartUtil::tstring & taggedValue )
	{
		itsIo << _T("<input type=text name=\"") << tag << _T("\"")
			  << _T(" value=\"") << taggedValue << _T("\" >\n");
	}

	//-------------------------------------------------------------------------






	// put( WidgetCheckBox * wcb )
	//
	template< class Parent, class MessageMapPolicy >
	void put( SmartWin::WidgetCheckBox< Parent, MessageMapPolicy > * wcb )
	{
		itsIo << "<input type=checkbox ";
		if ( wcb->getChecked() )
		{
			itsIo << "checked ";
		}
		itsIo << "value= \"" << wcb->getText() << "\" > " << wcb->getText() << " </input>\n";
	}

	//-------------------------------------------------------------------------

	// put( WidgetSlider * inSlider )
	// <input type="text" value="12" >
	//
	template< class Parent, class MessageMapPolicy >
	void put( SmartWin::WidgetSlider< Parent, MessageMapPolicy > * inSlider )
	{
		itsIo << "<input type=text value=\""
			  << inSlider->getPosition()
			  << "\" >\n";
	}

	//-------------------------------------------------------------------------

	// put( WidgetSpinner * inSpinner )
	// <input type=text value="100" >
	//
	template< class Parent, class MessageMapPolicy >
	void put( SmartWin::WidgetSpinner< Parent, MessageMapPolicy > * inSpinner )
	{
		itsIo << "<input type=text value=\""
			  << inSpinner->getValue()
			  << "\" >\n";
	}

	//-------------------------------------------------------------------------

	// put( WidgetComboBox * wcb )
	//
	template< class Parent, class MessageMapPolicy >
	void put( SmartWin::WidgetComboBox< Parent, MessageMapPolicy > * wcb )
	{
		itsIo << "<select>\n";
		int itemcount = wcb->getCount();
		int selectdex = wcb->getSelectedIndex();

		for ( int i = 0; i < itemcount; i++ )
		{
			itsIo << "<option value=\"" << wcb->getValue( i ) << "\" ";
			if ( i == selectdex )
			{
				itsIo << "selected "; // xhtml only:  << =\"selected\" " ;
			}
			itsIo << "> " << wcb->getValue( i ) << " </option>\n";
		}
		itsIo << "</select>\n\n";
	}

	//-------------------------------------------------------------------------

	// put( WidgetDataGrid wl )
	//
	template< class Parent, class MessageMapPolicy >
	void put( SmartWin::WidgetDataGrid< Parent, MessageMapPolicy > * wl )
	{
		int rows = wl->getRowCount();
		int cols = wl->getColumnCount();

		itsIo << "<table>" << endl;
		// itsIo << "<table WIDTH=\"100%\" BORDER=\"1\" CELLSPACING=\"1\" CELLPADDING=\"1\">"

		// Table header
		itsIo << "<tr>" << endl;
		for ( int c = 0; c < cols; c++ )
		{
			itsIo << "<th> " << wl->getColumnName( c ) << " </th>" << endl;
		}
		itsIo << "</tr>" << endl;

		// Table contents
		for ( int r = 0; r < rows; r++ )
		{
			itsIo << "<tr>" << endl;
			for ( int c = 0; c < cols; c++ )
			{
				itsIo << "<td> " << wl->getCellText( c, r ) << " </td>" << endl;
			}
			itsIo << "</tr>" << endl;
		}

		itsIo << "</table>" << endl;
	}

	//-------------------------------------------------------------------------

	// put( WidgetTreeView wtv )
	//
	template< class Parent, class MessageMapPolicy >
	void put( SmartWin::WidgetTreeView< Parent, MessageMapPolicy > * wtv )
	{
		TreeViewNode dummy, root;
		if ( wtv->getNode( dummy, TVGN_ROOT, root ) )
		{
			itsIo << "\n";
			html_tree_put( root, wtv );
		}
	}

	//-------------------------------------------------------------------------

private:

	void indent( int spaces )
	{
		for ( int i = 0; i < spaces; i++ )
			itsIo << " ";
	}

	//-------------------------------------------------------------------------

	// put( WidgetTreeView wtv )
	/*
	For example:
	<ul>

	<li> node1 </li>

	<li> node2 </li>
		<ul>
		<li> node21 </li>
		<li> node22 </li>
		</ul>

	</ul>
	*/
	//
	template< class Parent, class MessageMapPolicy >
	void html_tree_put( TreeViewNode & paramNode,
				SmartWin::WidgetTreeView< Parent, MessageMapPolicy > * wtv,
				int depth = 0 )
	{
		indent( depth * 2 );
		itsIo << "<ul>\n";
		TreeViewNode childNode, loopNode = paramNode;
		do
		{
			indent( depth * 2 );
			itsIo << "<li> " << wtv->getText( loopNode ) << " " << "</li>\n";

			if ( wtv->getNode( loopNode, TVGN_CHILD, childNode ) )
			{
				html_tree_put( childNode, wtv, depth + 1 );
			}
		} while ( wtv->getNode( loopNode, TVGN_NEXT, loopNode ) );

		indent( depth * 2 );
		itsIo << "</ul>\n";
	}

	//-------------------------------------------------------------------------
};

}

#endif