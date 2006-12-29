#pragma once

#ifndef __IMAGE_H__
#define __IMAGE_H__

#include <vector>
#include "File.h"
#include "Item.h"

namespace kTransfer3 {
  class Image(const Stamina::StringRef &name = ""): public File(name, NULL) {
  public:
    Image() {
      _type = enType::typeImage;
    }
  };

  typedef std::vector<Image*> tImages;
};

#endif /*__IMAGE_H__*/