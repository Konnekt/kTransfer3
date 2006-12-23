#ifndef __ITEM_H__
#define __ITEM_H__

#include "stdafx.h"
#include "State.h"
#include <windows.h>

class Item {
public: 
  enum enState {
    stNone,
    stOffline,
    stActive,
    stPaused,
    stCompleted,
    stTransfered,
    stStopped,
    stStarted,
    stError,
    stAborted,
    stIgnored
  };

public:
  Item() {
    _ref++;
    LARGE_INTEGER li;
    QueryPerformanceCounter(&li);
    _id = ((_ref << 16) | (li.LowPart & 0xFFFF));
  }
  virtual inline UINT getID() {
    return _id;
  }
  virtual inline enState getState() {
    return _state;
  }
  virtual inline void setState(enState state) {
    _state = state;
  }

private:
  UINT _id;
  static UINT _ref;

protected:
  enState _state;
};

#endif /*__ITEM_H__*/