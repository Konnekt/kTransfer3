// $Revision: 1.6 $
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
#ifndef XmlDocument_H
#define XmlDocument_H

#include <string>
#include <iosfwd>
#include <memory>
#include "XmlNode.h"
#include "XmlNamespace.h"
#include "XmlAttribute.h"
#include "SmartUtil.h"

namespace SmartXML
{
	class XmlDocument;

	/// An XML document can be written to any ostream through usage of the insertion operator (<<)
	std::basic_ostream< TCHAR > & operator <<( std::basic_ostream< TCHAR > &, const XmlDocument & writer );

	/// Encapsulates an XML document
	/** You cannot have an XmlNode (which kind of like is the "content" of an XML document) without having an XML document.
	  * This class encapsulates an XML document in most ways such that all manipulation of XML documents can (and should) be done through this class!
	  */
	class XmlDocument
	{
		friend std::basic_ostream< TCHAR > & operator <<( std::basic_ostream< TCHAR > &, const XmlDocument & writer );
	private:
		SmartUtil::tstring itsHeader;
		XmlNode::XmlNodePtr itsDocumentNode;
		mutable bool isNormalized; // Only used while serializing to indicate if we should "indent" output or not...

		SmartUtil::tstring loadHeader( std::basic_istream< TCHAR > & stream );
		XmlNode::XmlNodePtr parseNode( const SmartUtil::tstring & nodeBuffer, const SmartUtil::tstring & contentBuffer, XmlNode * parent );
		XmlNode::XmlNodePtr loadDocumentNode( std::basic_istream< TCHAR > & stream );
		XmlNode::XmlNodePtr loadNodes( std::basic_istream< TCHAR > & stream, XmlNode * parent );

	public:
		/// Returns the node with the given "DNA" code, see http://www.geocities.com/polterguy1000/ for a description of what a "DNA" code is
		XmlNode * findNode( const std::vector< unsigned int > & dnaCode );

		/// Type of XmlDocumentPtr;
		typedef boost::shared_ptr< XmlDocument > XmlDocumentPtr;

		/// Static constructor taking an istream from which it loads the document from.
		static XmlDocumentPtr createDocument( std::basic_istream< TCHAR > & stream );

		/// Constructor creating a new XmlDocument object
		/** Since all XML documents must have a "root node" this constructor ensures that the document is created with a root node and
		  * that no XmlDocument can exist WITHOUT an XML Document Node
		  */
		XmlDocument( const SmartUtil::tstring & docNodeName,
			const SmartUtil::tstring & docNodeContent,
			const SmartUtil::tstring & docNodeNamespace,
			const SmartUtil::tstring & version = _T( "1.0" ),
			const SmartUtil::tstring & encoding = _T( "utf-8" ) );

		/// Constructor creating an XML document from a stream
		/** XML document is loaded from the given stream and built up from that
		  */
		explicit XmlDocument( std::basic_istream< TCHAR > & stream );

		/// Returns the "document node"
		/** The document node is the "root node" or the node which is the "ancestor" of all other nodes in the document.
		  * All manipulation of the document is done through the "document node", adding nodes, deleting nodes and so on, but the "document node" cannot
		  * be changed after the document is created!
		  */
		XmlNode::XmlNodePtr getDocumentNode();
		const XmlNode::XmlNodePtr getDocumentNode() const;

		/// Returns a string representation of the XML document
		/** Returns a string representation of the XML document after it has been created!
		  */
		SmartUtil::tstring toString() const;

		/// Returns a string representation of the XML document
		/** Returns a string representation of the XML document after it has been created!
		  */
		SmartUtil::tstring toNormalizedString() const;
	};
}

#endif
