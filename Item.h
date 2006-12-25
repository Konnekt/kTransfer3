#ifndef __ITEM_H__
#define __ITEM_H__

#include "stdafx.h"
#include "State.h"
#include <windows.h>
#include <vector>

typedef std::vector<class Item*> tItem;

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
  enum enType {
    tFile = 1,
    tDirectory,
    tImage,
    tOther
  };

public:
  Item() {
    Stamina::LockerCS locker(_locker);

    // generacja losowego id
    _ref++;
    LARGE_INTEGER li;
    QueryPerformanceCounter(&li);
    // ref z li.LowPart powinny stworzyæ naprawdê unikalny id
    _id = ((_ref << 16) | (li.LowPart & 0xFFFF));

    // od tej chwili domysln¹ nazw¹ itemu jest jego id, zapisany numerycznie
    char buff[12];
    _name = itoa(_id, buff, 10);
  }
  virtual inline UINT getID() {
    return _id;
  }

  virtual inline enState getState() {
    Stamina::LockerCS locker(_locker);

    return _state;
  }

  virtual inline void setState(enState state) {
    Stamina::LockerCS locker(_locker);

    _state = state;
  }

  virtual inline enType getType() {
    return _type;
  }

  virtual inline bool setName(const Stamina::StringRef &name) {
    Stamina::LockerCS locker(_locker);

    _name = name;
    return true;
  }

  virtual inline Stamina::String getName() {
    Stamina::LockerCS locker(_locker);

    return _name;
  }

private:
  UINT _id;
  static UINT _ref;

protected:
  enState _state;
  enType _type;
  Stamina::String _name;

  Stamina::CriticalSection_w32 _locker;
};

UINT Item::_ref = 0;

#endif /*__ITEM_H__*/