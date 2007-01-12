// $Revision: 1.4 $
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
#ifndef XmlAttribute_H
#define XmlAttribute_H

#include <string>
#include <iosfwd>
#include "SmartUtil.h"

/// Contains the SmartXML "library" which is included in the SmartSOAP library
namespace SmartXML
{
	// Forward decalring friends
	class XmlDocument;
	class XmlNode;
	class XmlNode;

	std::basic_ostream< TCHAR > & operator <<( std::basic_ostream< TCHAR > &, const XmlDocument & writer );

	/// Encapsulates an XML attribute
	/** class for encapsulating an XML attribute
	  * class is immutable (meaning values can't change after object is created)
	  */
	class XmlAttribute
	{
		friend class XmlNode;
		friend std::basic_ostream< TCHAR > & operator <<( std::basic_ostream< TCHAR > &, const XmlDocument & writer );
	private:
		SmartUtil::tstring itsName;
		SmartUtil::tstring itsContent;
		SmartUtil::tstring itsNamespace;
		XmlNode * itsParent;

	public:
		/// Constructor creating an XML attribute with the given name and the given content.
		/** Created the attribute in the DEFAULT namespace
		  */
		XmlAttribute( XmlNode * parentNode, const SmartUtil::tstring & name, const SmartUtil::tstring & content );

		/// Constructor creating an XML attribute with the given name and the given content.
		/** Created the attribute in the GIVEN namespace
		  */
		XmlAttribute( XmlNode * parentNode, const SmartUtil::tstring & name, const SmartUtil::tstring & content, const SmartUtil::tstring & inNamespace );

		/// Returns the namespace of the attribute
		const SmartUtil::tstring & getNamespace() const;

		/// Returns the namespace of the attribute
		const SmartUtil::tstring & getNamespaceAlias() const;

		/// Returns the name of the attribute
		const SmartUtil::tstring & getName() const;

		// Returns the content of the attribute
		const SmartUtil::tstring & getContent() const;
	};
}

#endif
