#ifndef __ITEM_H__
#define __ITEM_H__

#include "stdafx.h"
#include "State.h"

class Item {
public:
  Item() {
    _ref++;
    _id = ((_ref << 16) | (GetTickCount() & 0xFFFF));
  }
  virtual inline int getID() {
    return _id;
  }

public:
  State state;

private:
  int _id;
  static int _ref;
};

#endif /*__ITEM_H__*/