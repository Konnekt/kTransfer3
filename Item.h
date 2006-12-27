#pragma once

#ifndef __ITEM_H__
#define __ITEM_H__

#include "stdafx.h"
#include <windows.h>
#include <vector>

namespace kTransfer3 {
  class Item: public iObject {
  public:
    STAMINA_OBJECT_CLASS_VERSION(Item, iObject, Version(0,1,0,0));

  public: 
    enum enState {
      stUnknown,
      stWaitingForConn,
      stQueued,
      stFinished,
      stActive,
      stError,
      stAborted,
      stIgnored
    };
    enum enType {
      typeFile = 1,
      typeDirectory,
      typeImage,
      typeOther
    };

  public:
    Item(const Stamina::StringRef &name = "") {
      Stamina::LockerCS locker(_locker);

      // generacja losowego id
      _ref++;
      LARGE_INTEGER li;
      QueryPerformanceCounter(&li);
      // ref z li.LowPart powinny stworzyæ naprawdê unikalny id
      _id = (_ref << 16) | (li.LowPart & 0xFFFF);

      // od tej chwili domysln¹ nazw¹ itemu jest jego id, zapisany numerycznie
       _name = !name.length() ? inttostr(_id) : name;
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

    virtual inline void setName(const Stamina::StringRef &name) {
      Stamina::LockerCS locker(_locker);

      _name = name;
    }

    virtual inline Stamina::String getName() {
      Stamina::LockerCS locker(_locker);

      return _name;
    }

  private:
    static UINT _ref;
    UINT _id;

  protected:
    Stamina::String _name;
    enState _state;
    enType _type;

    Stamina::CriticalSection _locker;
  };

  typedef std::vector<Item*> tItems;

  UINT Item::_ref = 0;
};

#endif /*__ITEM_H__*/