#ifndef __IMAGE_H__
#define __IMAGE_H__

#include "stdafx.h"
#include <vector>

#include "File.h"

typedef std::vector<class Image *> tImages;

class Image: public File {
public:
  Image() {
    _parent = NULL;
  }
};
#endif /*__IMAGE_H__*/