#include "stdafx.h"
#include "File.h"

namespace kTransfer3 {
  String File::getPath() {
    if (_parent_dir) {
      String path = _parent_dir->getPath() + "\\" + getName();
      return path;
    }
  }
};