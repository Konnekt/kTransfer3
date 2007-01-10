/**
  *  Transfer class - Image
  *
  *  Licensed under The GNU Lesser General Public License
  *  Redistributions of files must retain the above copyright notice.
  *
  *  @filesource
  *  @copyright    Copyright (c) 2006-2007 Ursus6
  *  @link         svn://konnekt.info/ktransfer3/ kTransfer3 plugin SVN Repo
  *  @version      $Revision$
  *  @modifiedby   $LastChangedBy$
  *  @lastmodified $Date$
  *  @license      http://creativecommons.org/licenses/LGPL/2.1/
  */

#pragma once

#ifndef __IMAGE_H__
#define __IMAGE_H__

#include <vector>
#include "File.h"
#include "Item.h"

namespace kTransfer3 {
  class Image(const StringRef &name = "", Transfer* parent): public File(name, parent, NULL): public Item(38, name, parent) {
  public:
    Image() {
    }
  };

  typedef std::vector<Image*> tImages;
};

#endif /*__IMAGE_H__*/