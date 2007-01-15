// $Revision: 1.3 $
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
#ifndef XmlNamespace_H
#define XmlNamespace_H

#include <iosfwd>
#include <string>
#include "SmartUtil.h"

namespace SmartXML
{
	class XmlDocument;

	/// An XML namespace can be written to any ostream through usage of the insertion operator (<<)
	std::basic_ostream< TCHAR > & operator <<( std::basic_ostream< TCHAR > &, const XmlDocument & writer );

	/// Encapsulates an XML namespace
	/** class for encapsulating an XML namespace
	  * class is immutable (meaning values can't change after object is created)
	  */
	class XmlNamespace
	{
		friend std::basic_ostream< TCHAR > & operator <<( std::basic_ostream< TCHAR > &, const XmlDocument & writer );
	private:
		SmartUtil::tstring itsNamespace;
		SmartUtil::tstring itsAlias;
	public:
		/// Constructor creating an XML namesapce with the given name and the given alias.
		explicit XmlNamespace( const SmartUtil::tstring & nameSpace, const SmartUtil::tstring & alias = _T( "" ) );

		/// Returns the name of the namespace
		const SmartUtil::tstring & getNamespace() const;

		// Returns the alias of the namespace
		const SmartUtil::tstring & getAlias() const;
	};
}

#endif
