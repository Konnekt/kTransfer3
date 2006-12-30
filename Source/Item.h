#pragma once

#ifndef __ITEM_H__
#define __ITEM_H__

#include <windows.h>
#include "Transfer.h"
#include <vector>

namespace kTransfer3 {
  typedef std::vector<Item*> tItems;

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
    Item(const Stamina::StringRef &name = "", Item* parent = NULL) {
      Stamina::LockerCS locker(_locker);

      // generacja losowego id
      _ref++;
      LARGE_INTEGER li;
      QueryPerformanceCounter(&li);
      // ref z li.LowPart powinny stworzy� naprawd� unikalny id
      _id = (_ref << 16) | (li.LowPart & 0xFFFF);

      // od tej chwili domysln� nazw� itemu jest jego id, zapisany numerycznie
       _name = !name.length() ? inttostr(_id) : name;

       _parent = parent;
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

    virtual inline Item* getParent() {
      return _parent;
    }

    virtual Stamina::String getPath();

  private:
    static UINT _ref;
    UINT _id;

  protected:
    Stamina::String _name;
    enState _state;
    enType _type;

    Item* _parent;

    Stamina::CriticalSection _locker;
  };

  UINT Item::_ref = 0;
};



#endif /*__ITEM_H__*/