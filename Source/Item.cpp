#include "stdafx.h"
#include "Item.h"

namespace kTransfer3 {
  Stamina::String Item::getPath() {
    if (((Stamina::iObject*)getParent())->getClass().getName() == "Item") {
      return getParent()->getPath() + "\\" + getName();
    }
    else {
      return ((Transfer *)getParent())->getSavePath() + "\\" + getName();
    }
  }
};
