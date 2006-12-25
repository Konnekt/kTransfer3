#ifndef __IMAGE_H__
#define __IMAGE_H__

#include "stdafx.h"
#include <vector>
#include "File.h"
#include "Item.h"

typedef std::vector<class Image *> tImages;

class Image: public File {
public:
  Image() {
    _type = enType::tImage;
    _parent = NULL;
  }
};
#endif /*__IMAGE_H__*/