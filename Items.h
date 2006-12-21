#ifndef __ITEMS_H__
#define __ITEMS_H__

#include "Directory.h"
#include <vector>
#include "stdafx.h"

class Items: public Directory {
public:
  inline Stamina::String getRootPath() {
    return _root_path;
  }
  inline void setRootPath(const Stamina::StringRef &path) {
    _root_path = path;
  }

private:
  Stamina::String _root_path;

protected:
};

#endif /*__ITEMS_H__*/