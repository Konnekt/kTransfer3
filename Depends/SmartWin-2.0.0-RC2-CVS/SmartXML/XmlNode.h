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
#ifndef XmlNode_H
#define XmlNode_H

#include <string>
#include <vector>
#include <iosfwd>
#include <boost.h>
#include "XmlAttribute.h"
#include "XmlNamespace.h"
#include "XmlNode.h"
#include "SmartUtil.h"

namespace SmartXML
{
	// Forward declaring friends
	class XmlDocument;

	/// An XML node can be written to any ostream through usage of the insertion operator (<<)
	std::basic_ostream< TCHAR > & operator <<( std::basic_ostream< TCHAR > &, const XmlDocument & writer );

	/// Encapsulates an XML node in an XML document
	/** All interaction (deletion, adding and so on) of an XML document is been done through this class.
	  * Though you must have an XML document to have an XML node this class can be seen as the "heart" of the SmartXML library!
	  */
	class XmlNode
	{
		friend std::basic_ostream< TCHAR > & operator <<( std::basic_ostream< TCHAR > &, const XmlDocument & writer );
		friend class XmlDocument;
	private:
		SmartUtil::tstring itsName;
		SmartUtil::tstring itsContent;
		SmartUtil::tstring itsNamespace;
		std::vector< XmlNode > itsChildren;
		std::vector< XmlNamespace > itsNamespaces;
		std::vector< XmlAttribute > itsAttributes;
		XmlNode * itsParent;

	public:
		// TODO: Bad solution...
		// Must be public to allow vector to contain items directly and not pointers
		XmlNode( const XmlNode & rhs );

		// Typedefs for iterational access
		typedef std::vector< XmlNode >::iterator node_iterator;
		typedef std::vector< XmlNode >::const_iterator const_node_iterator;

		typedef std::vector< XmlAttribute >::iterator attribute_iterator;
		typedef std::vector< XmlAttribute >::const_iterator const_attribute_iterator;

		typedef std::vector< XmlNamespace >::iterator namespace_iterator;
		typedef std::vector< XmlNamespace >::const_iterator const_namespace_iterator;

		typedef boost::shared_ptr< XmlNode > XmlNodePtr;

	private:
		// Used internally ONLY from XmlDocument (which is friend)
		// This is the ONLY way to create a node with NO PARENT
		XmlNode( const SmartUtil::tstring & name, const SmartUtil::tstring & content, const SmartUtil::tstring & nameSpace );
		static XmlNodePtr createNode( const SmartUtil::tstring & name, const SmartUtil::tstring & content, const SmartUtil::tstring & nameSpace );

		// Internally used constructors, to create an XmlNode from outside use the insertNode function
		static XmlNodePtr createNode( XmlNode * parent, const SmartUtil::tstring & name, const SmartUtil::tstring & content, const SmartUtil::tstring & nameSpace );
		XmlNode( XmlNode * parent, const SmartUtil::tstring & name, const SmartUtil::tstring & content, const SmartUtil::tstring & nameSpace );
		XmlNode & addChild( const XmlNode & node );

	public:
		/// Returns a vector of integers defining the "DNA" code of the Node, see http://www.geocities.com/polterguy1000/ for a thorough explenation
		/** Useful for doing "vector" math on nodes in a DOM document.
		  * You can calculate order by doing a comparison of the DNA codes of two different nodes, you can calculate distance to document node,
		  * you can calculate minimum number of sibblings to all ancestors etc.
		  * Also you can store a "pointer" to a node and use for later retrieving that same node as long as there has not been inserted nodes
		  * at any of its ancestors "earlier" in the hierarchy.
		  * You can also use the DNA code to exactly calculate the placement of the node in the hierarchy.
		  * Basically a "Genetic Tree" is an easy way to describe a graph in a linear form.
		  */
		std::vector< unsigned int > getDNACode() const;

		/// Returns the human readable content of the XML node, throws if node does not allow for content, e.g. have child nodes
		const SmartUtil::tstring getContent() const;

		/// Returns the name of the XML node
		const SmartUtil::tstring & getName() const;

		/// Returns the namespace of the XML node
		const SmartUtil::tstring & getNamespaceAlias() const;

		/// Returns the parent node
		XmlNode * getParent();
		const XmlNode * getParent() const;

		/// Returns the namespace of the attribute
		const SmartUtil::tstring & getNamespace() const;

		/// begin of children iterator
		node_iterator children_begin();

		/// end of children iterator
		node_iterator children_end();

		const_node_iterator children_begin() const;
		const_node_iterator children_end() const;

		/// begin of attributes
		attribute_iterator attributes_begin();

		/// end of attributes
		attribute_iterator attributes_end();

		const_attribute_iterator attributes_begin() const;
		const_attribute_iterator attributes_end() const;

		/// begin of namespaces in node
		namespace_iterator namespaces_begin();

		/// end of namespaces in node
		namespace_iterator namespaces_end();

		const_namespace_iterator namespaces_begin() const;
		const_namespace_iterator namespaces_end() const;

		/// Subscript operator returning the n'th children node
		XmlNode & operator[] ( size_t idx );
		const XmlNode & operator[] ( size_t idx ) const;

		/// Returns the number of generation one children the node has
		size_t children_size() const
		{ return itsChildren.size();
		}

		/// Sets the name of the XmlNode
		XmlNode & setName( const SmartUtil::tstring & name )
		{ itsName = name; return * this;
		}

		/// Sets the content of the XmlNode
		XmlNode & setContent( const SmartUtil::tstring & content )
		{ itsContent = content; return * this;
		}

		/// Sets the namespace of the XmlNode
		XmlNode & setNamespace( const SmartUtil::tstring & nameSpace )
		{ itsNamespace = nameSpace; return * this;
		}

		/// Adds a child to the children list and returns a reference to that child
		XmlNode & addChild( const SmartUtil::tstring & name, const SmartUtil::tstring & content, const SmartUtil::tstring & nameSpace );

		/// Adds an attribute to the XmlNode and returns the node
		XmlNode & addAttribute( const XmlAttribute & attribute );

		/// Adds a namespace to the XmlNode
		XmlNode & addNamespace( const XmlNamespace & nameSpace );

		/// Returns the value of the attribute with the given name
		/** Note that one attribute may have two attributes with the same name existing in the same namespace,
		  * this function will just return the FIRST attribute it finds with the given name.
		  * If you need to filter on the namespace in addition then use the findAttributeComplete function that takes
		  * the namespace in addition instead!
		  */
		SmartUtil::tstring findAttribute( const SmartUtil::tstring & name ) const;

		/// Returns the attribute with the given name
		/** This one just returns the FIRST attribute it finds with the given name!
		  * This means there MIGHT be problems regarding to attributes with the same name existing in different namespaces,
		  * to play safe you would want to look for namespace with a given name AND existing in a given namespace!
		  * Though this one is included for really simple XML documents due to that it's way more speedy!!
		  */
		XmlAttribute * findAttributeComplete( const SmartUtil::tstring & name );

		/// Returns the attribute with the given name
		/** This one just returns the FIRST attribute it finds with the given name!
		  * This means there MIGHT be problems regarding to attributes with the same name existing in different namespaces,
		  * to play safe you would want to look for namespace with a given name AND existing in a given namespace!
		  * Though this one is included for really simple XML documents due to that it's way more speedy!!
		  */
		const XmlAttribute * findAttributeComplete( const SmartUtil::tstring & name ) const;

		/// Returns the attribute with the given name
		/** This one searches for the given attribute filtering on both name and the given namespace.
		  * Note that the namespace given here is NOT the "alias" but the COMPLETE namespace, meaning if you have
		  * a namespace that looks like this: xmlns:myNs="thomas hansen" you should supply "thomas hansen" as the nameSpace
		  * and NOT the "myNs"...
		  * It will recursively look upwards in the DOM until it finds the namespace for the attribute meaning it might take some time to finish!
		  */
		XmlAttribute * findAttributeComplete( const SmartUtil::tstring & name, const SmartUtil::tstring & nameSpace );

		/// Returns the attribute with the given name
		/** This one searches for the given attribute filtering on both name and the given namespace.
		  * Note that the namespace given here is NOT the "alias" but the COMPLETE namespace, meaning if you have
		  * a namespace that looks like this: xmlns:myNs="thomas hansen" you should supply "thomas hansen" as the nameSpace
		  * and NOT the "myNs"...
		  * It will recursively look upwards in the DOM until it finds the namespace for the attribute meaning it might take some time to finish!
		  */
		const XmlAttribute * findAttributeComplete( const SmartUtil::tstring & name, const SmartUtil::tstring & nameSpace ) const;
	};
}

#endif
