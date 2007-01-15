// $Revision$
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

#include <fstream>
#include <string>
#include <memory>
#include <iostream>
#include <sstream>
#include "XmlSystemHeaders.h"
#include "XmlDocument.h"
#include "XmlAttribute.h"
#include "XmlNamespace.h"
#include "XmlNode.h"
#include "xCeptionXML.h"

namespace SmartXML
{
SmartUtil::tstring XmlDocument::loadHeader( std::basic_istream< TCHAR > & stream )
{
	TCHAR extracted[2];
	SmartUtil::tstring header;
	bool headerFound = false;
	for ( int n = 0; n < 2 || headerFound; )
	{
		extracted[n] = stream.get();
		header.push_back( extracted[n] );
		if ( header.size() == 2 && header.substr( 0, 2 ) != _T( "<?" ) )
		{
			stream.putback( extracted[1] );
			stream.putback( extracted[0] );
			return _T( "" );
		}
		else if ( header.size() == 2 && header.substr( 0, 2 ) == _T( "<?" ) )
		{
			headerFound = true;
		}
		if ( header.size() > 2 && header.substr( header.size() - 2, 2 ) == _T( "?>" ) )
			break;
		if ( header.size() > 100 )
			throw xCeptionXML( "Header of XML document is probably corrupted" );
		if ( n < 1 )
			++n;
	}
	return header;
}

XmlNode::XmlNodePtr XmlDocument::parseNode( const SmartUtil::tstring & nodeBuffer, const SmartUtil::tstring & contentBuffer, XmlNode * parent )
{
	SmartUtil::tstring nodeAndOrNamespace;
	SmartUtil::tstring::const_iterator idx = nodeBuffer.begin() + 1; // Skipping the initial "<"
	while ( true )
	{
		if ( * idx == ' ' || * idx == '>' )
			break;
		nodeAndOrNamespace.push_back( * idx );
		++idx;
	}
	size_t indexOfColon = nodeAndOrNamespace.find( ':' );
	SmartUtil::tstring nameSpace;
	if ( indexOfColon != SmartUtil::tstring::npos )
	{
		nameSpace = nodeAndOrNamespace.substr( 0, indexOfColon );
		nodeAndOrNamespace = nodeAndOrNamespace.substr( indexOfColon + 1 );
	}
	SmartUtil::tstring name = nodeAndOrNamespace;
	XmlNode::XmlNodePtr retVal = parent == 0 ? XmlNode::createNode( name, contentBuffer, nameSpace )
		: XmlNode::createNode( parent, name, contentBuffer, nameSpace );
	while ( * idx != '>' )
	{
		// We've got attributes and/or namespaces
		while ( true )
		{
			// Skipping initial whitespace before namespace/attribute
			if ( * idx != ' ' || idx == nodeBuffer.end() )
				break;
			++idx;
		}
		if ( idx == nodeBuffer.end() )
			break;
		if ( * idx == '/' )
			break; // Node closed immediately
		SmartUtil::tstring attrNamespaceBuffer;
		SmartUtil::tstring contentOfAttrOrNamespace;
		bool seenOpeningFnutt = false, seenClosingFnutt = false;
		while ( !seenClosingFnutt && idx != nodeBuffer.end() )
		{
			attrNamespaceBuffer.push_back( * idx );
			if ( * idx == '"' || * idx == '\'' )
			{
				if ( seenOpeningFnutt == false )
					seenOpeningFnutt = true;
				else
					seenClosingFnutt = true;
			}
			else if ( seenOpeningFnutt && !seenClosingFnutt )
				contentOfAttrOrNamespace.push_back( * idx );
			++idx;
		}
		if ( idx == nodeBuffer.end() )
			break;
		SmartUtil::tstring beforeEqualSign = attrNamespaceBuffer.substr( 0, attrNamespaceBuffer.find( _T( "=" ) ) );
		size_t indexOfColon = beforeEqualSign.find( _T( ":" ) );
		if ( ( indexOfColon == SmartUtil::tstring::npos || beforeEqualSign.substr( 0, indexOfColon ) != _T( "xmlns" ) ) && beforeEqualSign != _T( "xmlns" ) )
		{
			// Attribute
			if ( indexOfColon != SmartUtil::tstring::npos )
			{
				XmlAttribute attr( retVal.get(), beforeEqualSign.substr( indexOfColon + 1 ), contentOfAttrOrNamespace, beforeEqualSign.substr( 0, indexOfColon ) );
				retVal->addAttribute( attr );
			}
			else
			{
				XmlAttribute attr( retVal.get(), beforeEqualSign, contentOfAttrOrNamespace );
				retVal->addAttribute( attr );
			}
		}
		else
		{
			// Namespace
			XmlNamespace nameSpace( contentOfAttrOrNamespace, indexOfColon == SmartUtil::tstring::npos ? beforeEqualSign : beforeEqualSign.substr( indexOfColon + 1 ) );
			retVal->addNamespace( nameSpace );
		}
	}
	return retVal;
}

XmlNode::XmlNodePtr XmlDocument::loadDocumentNode( std::basic_istream< TCHAR > & stream )
{
	XmlNode::XmlNodePtr retVal = loadNodes( stream, 0 );
	return retVal;
}

XmlNode::XmlNodePtr XmlDocument::loadNodes( std::basic_istream< TCHAR > & stream, XmlNode * parent )
{
	SmartUtil::tstring nodeBuffer;
	bool foundOpeningAngleBracket = false;
	while ( nodeBuffer.size() == 0 )
	{
		while ( true )
		{
			TCHAR buffer = stream.get();
			if ( stream.eof() || !stream.good() )
				return XmlNode::XmlNodePtr();
			if ( !foundOpeningAngleBracket && buffer == _T( '<' ) )
			{
				TCHAR tmpChar = stream.get();
				if ( tmpChar != _T( '!' ) ) // We just SKIP comments...
					foundOpeningAngleBracket = true;
				stream.putback( tmpChar );
			}
			else if ( !foundOpeningAngleBracket )
				continue;
			nodeBuffer.push_back( buffer );
			if ( nodeBuffer[nodeBuffer.size() - 1] == _T( '>' ) )
				break;
		}
		size_t firstNotOfSpaces = nodeBuffer.find_first_not_of( _T( " \t\r\n" ) );
		nodeBuffer = nodeBuffer.substr( firstNotOfSpaces );
		if ( nodeBuffer.substr( 0, 2 ) == _T( "</" ) )
			return XmlNode::XmlNodePtr();
	}
	SmartUtil::tstring contentBuffer;
	if ( nodeBuffer.substr( nodeBuffer.size() - 2, 2 ) != _T( "/>" ) )
	{
		while ( true )
		{
			TCHAR buffer = stream.get();
			if ( stream.eof() )
				break;
			if ( buffer == _T( '<' ) )
			{
				stream.putback( _T( '<' ) );
				break;
			}
			contentBuffer.push_back( buffer );
		}
	}
	XmlNode::XmlNodePtr retVal = parseNode( nodeBuffer, contentBuffer, parent );
	if ( nodeBuffer.substr( nodeBuffer.size() - 2 ) != _T( "/>" ) ) // Only check for children if node isn't closed immediately!!
	{
		while ( true )
		{
			XmlNode::XmlNodePtr retValChild = loadNodes( stream, retVal.get() );
			if ( retValChild.get() )
				retVal->addChild( * retValChild );
			else
				break;
		}
	}
	return retVal;
}

XmlNode * XmlDocument::findNode( const std::vector< unsigned int > & dnaCode )
{
	XmlNode * retVal = itsDocumentNode.get();
	for ( std::vector< unsigned int >::const_iterator idx = ++dnaCode.begin();
		idx != dnaCode.end();
		++idx )
	{
		if ( retVal->itsChildren.size() < * idx )
			return 0; // MISSED!!
		retVal = & retVal->itsChildren[ * idx];
	}
	return retVal;
}

XmlDocument::XmlDocument( const SmartUtil::tstring & docNodeName,
	const SmartUtil::tstring & docNodeContent,
	const SmartUtil::tstring & docNodeNamespace,
	const SmartUtil::tstring & version,
	const SmartUtil::tstring & encoding )
	: itsHeader( _T( "<?xml version=\"" ) + version + _T( "\" encoding=\"" ) + encoding + _T( "\"?>" ) ),
	itsDocumentNode( XmlNode::createNode( docNodeName, docNodeContent, docNodeNamespace ) )
{}

XmlDocument::XmlDocumentPtr XmlDocument::createDocument( std::basic_istream< TCHAR > & stream )
{
	XmlDocumentPtr retVal( new XmlDocument( stream ) );
	return retVal;
}

XmlDocument::XmlDocument( std::basic_istream< TCHAR > & stream )
	: itsHeader( loadHeader( stream ) ),
	itsDocumentNode( loadDocumentNode( stream ) )
{}

XmlNode::XmlNodePtr XmlDocument::getDocumentNode()
{
	return itsDocumentNode;
}

const XmlNode::XmlNodePtr XmlDocument::getDocumentNode() const
{
	return itsDocumentNode;
}

std::basic_ostream< TCHAR > & operator <<( std::basic_ostream< TCHAR > & stream, const XmlDocument & document )
{
	// Embedded class to have embedded funtion for recursively parsing XML nodes
	class WriterHelper
	{
	public:
		bool isNormalized;

		void writeNode( const XmlNode & node, std::basic_ostream< TCHAR > & stream, int indentLevel )
		{
			if ( isNormalized )
			{
				// Making sure the indentlevel comes out right!
				// But only if "normalized" output is wanted...
				if ( indentLevel != 0 )
					stream << _T( "\r\n" );
				for ( int idxIndent = 0; idxIndent < indentLevel; ++ idxIndent )
				{
					stream << _T( "\t" );
				}
			}
			if ( node.getNamespaceAlias().size() == 0 )
				stream << _T( "<" ) << node.getName();
			else
				stream << _T( "<" ) << node.getNamespaceAlias() << _T( ":" ) << node.getName();
			for ( std::vector< XmlNamespace >::const_iterator iterNamespace = node.namespaces_begin();
				iterNamespace != node.namespaces_end();
				++iterNamespace )
			{
				if ( iterNamespace->getAlias().size() == 0 )
					stream << _T( " xmlns=\"" ) << iterNamespace->getNamespace() << _T( "\"" );
				else
					stream << _T( " xmlns:" ) << iterNamespace->getAlias() << _T( "=\"" ) << iterNamespace->getNamespace() << _T( "\"" );
			}
			for ( std::vector< XmlAttribute >::const_iterator iterAttributes = node.attributes_begin();
				iterAttributes != node.attributes_end();
				++iterAttributes )
			{
				stream << _T( " " ) << iterAttributes->getNamespaceAlias() << ( iterAttributes->getNamespaceAlias() != _T( "" ) ? _T( ":" ) : _T( "" ) ) << iterAttributes->getName() << _T( "=\"" ) << iterAttributes->getContent() << _T( "\"" );
			}
			if ( node.children_begin() == node.children_end() )
			{
				if ( node.getContent().size() == 0 )
					stream << _T( " />" );
				else
				{
					stream << _T( ">" ) << node.getContent();
					if ( node.getNamespaceAlias().size() == 0 )
						stream << _T( "</" ) << node.getName() << _T( ">" );
					else
						stream << _T( "</" ) << node.getNamespaceAlias() << _T( ":" ) << node.getName() << _T( ">" );
				}
			}
			else
			{
				stream << _T( ">" );
				++indentLevel;
				for ( std::vector< XmlNode >::const_iterator iterChildren = node.children_begin();
					iterChildren != node.children_end();
					++iterChildren )
				{
					writeNode( * iterChildren, stream, indentLevel );
				}
				--indentLevel;
				if ( isNormalized )
				{
					// Making sure the indentlevel comes out right!
					// But only if "normalized" output is wanted...
					stream << _T( "\r\n" );
					for ( int idxIndent = 0; idxIndent < indentLevel; ++ idxIndent )
					{
						stream << _T( "\t" );
					}
				}
				if ( node.getNamespaceAlias().size() == 0 )
					stream << _T( "</" ) << node.getName() << _T( ">" );
				else
					stream << _T( "</" ) << node.getNamespaceAlias() << _T( ":" ) << node.getName() << _T( ">" );
			}
		}
	};

	if ( document.itsHeader.size() != 0 )
		stream << document.itsHeader << std::endl;

	WriterHelper writer;
	writer.isNormalized = document.isNormalized;
	writer.writeNode( * document.itsDocumentNode, stream, 0 );
	return stream;
}

SmartUtil::tstring XmlDocument::toString() const
{
	isNormalized = false;
	std::basic_stringstream< TCHAR > ss;
	ss << * this;
	return ss.str();
}

SmartUtil::tstring XmlDocument::toNormalizedString() const
{
	isNormalized = true;
	std::basic_stringstream< TCHAR > ss;
	ss << * this;
	return ss.str();
}
}
