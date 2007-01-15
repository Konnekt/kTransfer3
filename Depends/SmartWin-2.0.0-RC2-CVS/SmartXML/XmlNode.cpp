// $Revision:$
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
#include <vector>
#include "XmlNode.h"
#include "XmlNamespace.h"
#include "XmlAttribute.h"
#include "xCeptionXML.h"

namespace SmartXML
{
XmlNode::XmlNode( const XmlNode & rhs )
	: itsName( rhs.itsName ),
	itsContent( rhs.itsContent ),
	itsNamespace( rhs.itsNamespace ),
	itsChildren( rhs.itsChildren ),
	itsNamespaces( rhs.itsNamespaces ),
	itsAttributes( rhs.itsAttributes ),
	itsParent( rhs.itsParent )
{
	for ( std::vector< XmlNode >::iterator iter = itsChildren.begin();
		iter != itsChildren.end();
		++iter )
	{
		iter->itsParent = this;
	}
	for ( std::vector< XmlAttribute >::iterator iter = itsAttributes.begin();
		iter != itsAttributes.end();
		++iter )
	{
		iter->itsParent = this;
	}
}

XmlNode::XmlNodePtr XmlNode::createNode( XmlNode * parent,
	const SmartUtil::tstring & name,
	const SmartUtil::tstring & content,
	const SmartUtil::tstring & nameSpace )
{
	XmlNodePtr retVal( new XmlNode( parent, name, content, nameSpace ) );
	return retVal;
}

XmlNode::XmlNodePtr XmlNode::createNode( const SmartUtil::tstring & name,
	const SmartUtil::tstring & content,
	const SmartUtil::tstring & nameSpace )
{
	XmlNodePtr retVal( new XmlNode( name, content, nameSpace ) );
	return retVal;
}

XmlNode::XmlNode( XmlNode * parent, const SmartUtil::tstring & name, const SmartUtil::tstring & content, const SmartUtil::tstring & nameSpace )
	: itsName( name ),
	itsContent( content ),
	itsNamespace( nameSpace ),
	itsParent( parent )
{
	if ( parent == 0 )
		throw xCeptionXML( "Can't have an XmlNode without a parent except for the DocumentNode of an XmlDocument" );
}

XmlNode::XmlNode( const SmartUtil::tstring & name, const SmartUtil::tstring & content, const SmartUtil::tstring & nameSpace )
	: itsName( name ),
	itsContent( content ),
	itsNamespace( nameSpace ),
	itsParent( 0 )
{}

// Private never exposed functions for "converting" XmlNode content to human readable string
// e.g. the &gt; will be converted to an '>' etc...
SmartUtil::tstring changeString( const SmartUtil::tstring & whatToConvert, const SmartUtil::tstring & whatToConvertItTo, const SmartUtil::tstring & whichStringToConvert )
{
	SmartUtil::tstring retVal = whichStringToConvert;
	while ( true )
	{
		size_t indexOfReplace = retVal.find( whatToConvert );
		if ( indexOfReplace == SmartUtil::tstring::npos )
			break; // Finished
		retVal.replace( indexOfReplace, whatToConvert.size(), whatToConvertItTo );
	}

	return retVal;
}

SmartUtil::tstring convertContentToHumanReadable( const SmartUtil::tstring & content )
{
	// TODO: Optimize...
	SmartUtil::tstring retVal = content;

	// XML særtegn
	retVal = changeString( _T( "&gt;" ), _T( ">" ), retVal );
	retVal = changeString( _T( "&lt;" ), _T( "<" ), retVal );
	retVal = changeString( _T( "&amp;" ), _T( "&" ), retVal );

	// TODO: Implement the rest of them...
	return retVal;
}

const SmartUtil::tstring XmlNode::getContent() const
{
	if ( itsChildren.size() > 0 )
		throw xCeptionXML( "Can't get content of XML node which have childrens" );
	return convertContentToHumanReadable( itsContent );
}

XmlNode & XmlNode::addChild( const SmartUtil::tstring & name, const SmartUtil::tstring & content, const SmartUtil::tstring & nameSpace )
{
	XmlNode node( this, name, content, nameSpace );
	itsChildren.push_back( node );
	return itsChildren[ itsChildren.size() - 1 ];
}

std::vector< unsigned int > XmlNode::getDNACode() const
{
	std::vector< unsigned int > retVal;
	const XmlNode * current = this;
	while ( current != 0 )
	{
		if ( current->itsParent == 0 )
		{
			retVal.insert( retVal.begin(), 0 );
		}
		else
		{
			unsigned int val = 0;
			for ( const_node_iterator idx = current->itsParent->children_begin();
				idx != current->itsParent->children_end();
				++idx, ++val )
			{
				if ( & ( * idx ) == current )
				{
					retVal.insert( retVal.begin(), val );
					break;
				}
			}
		}
		current = current->itsParent;
	}
	return retVal;
}

const SmartUtil::tstring & XmlNode::getName() const
{
	return itsName;
}

const SmartUtil::tstring & XmlNode::getNamespaceAlias() const
{
	return itsNamespace;
}

XmlNode * XmlNode::getParent()
{
	return itsParent;
}

const XmlNode * XmlNode::getParent() const
{
	return itsParent;
}

const SmartUtil::tstring & XmlNode::getNamespace() const
{
	const XmlNode * current = this;
	while ( current )
	{
		for ( XmlNode::const_namespace_iterator idx = current->namespaces_begin();
			idx != current->namespaces_end();
			++idx )
		{
			if ( itsNamespace == idx->getAlias() )
			{
				return idx->getNamespace();
			}
		}
		current = current->itsParent;
	}
	const static SmartUtil::tstring tmpRetVal = _T( "" );
	return tmpRetVal;
}

XmlNode::node_iterator XmlNode::children_begin()
{
	return itsChildren.begin();
}

XmlNode::node_iterator XmlNode::children_end()
{
	return itsChildren.end();
}

XmlNode::const_node_iterator XmlNode::children_begin() const
{
	return itsChildren.begin();
}

XmlNode::const_node_iterator XmlNode::children_end() const
{
	return itsChildren.end();
}

XmlNode::attribute_iterator XmlNode::attributes_begin()
{
	return itsAttributes.begin();
}

XmlNode::attribute_iterator XmlNode::attributes_end()
{
	return itsAttributes.end();
}

XmlNode::const_attribute_iterator XmlNode::attributes_begin() const
{
	return itsAttributes.begin();
}

XmlNode::const_attribute_iterator XmlNode::attributes_end() const
{
	return itsAttributes.end();
}

XmlNode::namespace_iterator XmlNode::namespaces_begin()
{
	return itsNamespaces.begin();
}

XmlNode::namespace_iterator XmlNode::namespaces_end()
{
	return itsNamespaces.end();
}

XmlNode::const_namespace_iterator XmlNode::namespaces_begin() const
{
	return itsNamespaces.begin();
}

XmlNode::const_namespace_iterator XmlNode::namespaces_end() const
{
	return itsNamespaces.end();
}

XmlNode & XmlNode::addChild( const XmlNode & node )
{
	itsChildren.push_back( node );
	return * this;
}

XmlNode & XmlNode::addAttribute( const XmlAttribute & attribute )
{
	itsAttributes.push_back( attribute );
	return * this;
}

XmlNode & XmlNode::addNamespace( const XmlNamespace & nameSpace )
{
	itsNamespaces.push_back( nameSpace );
	return * this;
}

XmlNode & XmlNode::operator[] ( size_t idx )
{
	return itsChildren[idx];
}

const XmlNode & XmlNode::operator[] ( size_t idx ) const
{
	return itsChildren[idx];
}

SmartUtil::tstring XmlNode::findAttribute( const SmartUtil::tstring & name ) const
{
	for ( const_attribute_iterator idx = attributes_begin();
		idx != attributes_end();
		++idx )
	{
		if ( idx->getName() == name )
			return idx->getContent();
	}
	return _T( "" ); // TODO: Return "null" value...
}

XmlAttribute * XmlNode::findAttributeComplete( const SmartUtil::tstring & name )
{
	for ( XmlNode::attribute_iterator idx = attributes_begin();
		idx != attributes_end();
		++idx )
	{
		if ( idx->getName() == name )
		{
			return & ( * idx );
		}
	}
	return 0;
}

const XmlAttribute * XmlNode::findAttributeComplete( const SmartUtil::tstring & name ) const
{
	for ( XmlNode::const_attribute_iterator idx = attributes_begin();
		idx != attributes_end();
		++idx )
	{
		if ( idx->getName() == name )
		{
			return & ( * idx );
		}
	}
	return 0;
}

XmlAttribute * XmlNode::findAttributeComplete( const SmartUtil::tstring & name, const SmartUtil::tstring & nameSpace )
{
	for ( XmlNode::attribute_iterator idx = attributes_begin();
		idx != attributes_end();
		++idx )
	{
		if ( idx->getName() == name && idx->getNamespace() == nameSpace )
		{
			return & ( * idx );
		}
	}
	return 0;
}

const XmlAttribute * XmlNode::findAttributeComplete( const SmartUtil::tstring & name, const SmartUtil::tstring & nameSpace ) const
{
	for ( XmlNode::const_attribute_iterator idx = attributes_begin();
		idx != attributes_end();
		++idx )
	{
		if ( idx->getName() == name && idx->getNamespace() == nameSpace )
		{
			return & ( * idx );
		}
	}
	return 0;
}
}
