#pragma once

#ifndef __IMAGE_H__
#define __IMAGE_H__

#include <vector>
#include "File.h"
#include "Item.h"

namespace kTransfer3 {
  class Image: public File {
  public:
    Image() {
      _type = enType::typeImage;
      _parent = NULL;
    }
  };

  typedef std::vector<Image*> tImages;
};

#endif /*__IMAGE_H__*/