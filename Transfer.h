#pragma once

#ifndef __TRANSFER_H__
#define __TRANSFER_H__

#include "Group.h"
#include "Item.h"

class Transfer: public Group, public Stamina::SharedObject<Stamina::iSharedObject> {
public:
  enum enType {
    typeNone,
    typeFile,
    typeFiles,
    typeFilesFolders,
    typeImage,
    typeOther
  };

public:
  STAMINA_OBJECT_CLASS_VERSION(Transfer, Stamina::iSharedObject, Stamina::Version(0,1,0,0));

  Transfer(enType type, int net = 0, int cnt = 0) {
    Stamina::LockerCS locker(_locker);

    _ref++;
    LARGE_INTEGER li;
    QueryPerformanceCounter(&li);
    _id = ((_ref << 16) | (li.LowPart & 0xFFFF));

    _type = type;
    _net = net;
    _cnt = cnt;
  }

  inline UINT getID() {
    return _id;
  }

  inline void setName(const Stamina::StringRef &name) {
    Stamina::LockerCS locker(_locker);

    _name = name;
  }
  inline Stamina::String getName() {
    Stamina::LockerCS locker(_locker);

    return _name;
  }

  inline __int64 getSpeed() {
    Stamina::LockerCS locker(_locker);

    return _last_speed; 
  }
  inline void setSpeed(__int64 bps) {
    Stamina::LockerCS locker(_locker);

    _last_speed = bps;
  }

  inline Stamina::Time64 getBeginTime() {
    Stamina::LockerCS locker(_locker);

    return _begin_time;
  }
  inline void setBeginTime(const Stamina::Time64 &time) {
    Stamina::LockerCS locker(_locker);

    _begin_time = time;
  }

  inline Stamina::String getSavePath() {
    Stamina::LockerCS locker(_locker);

    return _save_path;
  }
  inline void setSavePath(const Stamina::StringRef &path) {
    Stamina::LockerCS locker(_locker);

    _save_path = path;
  }

  inline bool addItem(Item* item) {
    Stamina::LockerCS locker(_locker);

    if (haveItem(item->getName(), item->getType())) {
      return false;
    }
    _items.push_back(item);
    return true;
  }

  inline Item* getItem(UINT id) {
    Stamina::LockerCS locker(_locker);

    tItem::iterator it = _items.begin();
    for (;it != this->_items.end(); it++) {
      if ((*it)->getID() == id) return (*it);
    }
    return NULL;
  }

  inline bool removeItem(UINT id) {
    Stamina::LockerCS locker(_locker);

    tItem::iterator it = _items.begin();
    for (;it != this->_items.end(); it++) {
      if ((*it)->getID() == id) {
        delete (*it);
        _items.erase(it); 
        return true;
      }
    }
    return false;
  }

  inline UINT getItemID(const Stamina::StringRef &name, Item::enType type) {
    Stamina::LockerCS locker(_locker);

    tItem::iterator it = _items.begin();
      for (;it != this->_items.end(); it++) {
        if (((*it)->getName() == name) && ((*it)->getType() == type)) {
          return (*it)->getID();
        }
      }
    return 0;
  }

  inline bool haveItem(const Stamina::StringRef &name, Item::enType type) {
    Stamina::LockerCS locker(_locker);

    return getItemID(name, type);
  }

  virtual inline UINT getItemsCount() {
    Stamina::LockerCS locker(_locker);

    return _items.size();
  }

  virtual inline Item* getItemT(UINT id_t) {
    Stamina::LockerCS locker(_locker);

    return _items[id_t];
  }

  inline bool setItemName(UINT id, const Stamina::StringRef &name) {
    Stamina::LockerCS locker(_locker);

    Item *item = getItem(id);
    if (haveItem(name, item->getType())) {
      return false;
    }
    tItem::iterator it = _items.begin();
      for (;it != this->_items.end(); it++) {
        if (((*it)->getID() == id)) return (*it)->setName(name);
      }
    return false;
  }
  
public:
  tItems _items;

private:
  static UINT _ref;
  UINT _id;

protected:
  Stamina::CriticalSection _locker;
  __int64 _last_speed;
  enType _type;

  int _cnt;
  int _net;

  Stamina::Time64 _begin_time;
  Stamina::String _save_path;
  Stamina::String _name;
};

typedef Stamina::SharedPtr<Transfer> oTransfer;

UINT Transfer::_ref = 0;

#endif /*__TRANSFER_H__*/