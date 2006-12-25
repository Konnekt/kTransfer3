#ifndef __TRANSFER_H__
#define __TRANSFER_H__

#include "stdafx.h"
#include "Group.h"
#include "State.h"
#include "Item.h"

typedef Stamina::SharedPtr<class Transfer> oTransfer;

class Transfer: public Group, public Stamina::SharedObject<Stamina::iSharedObject> {
public:
  enum enType {
    tNone,
    tOneFile,
    tMultiFile,
    tMultiFileFolders,
    tOther
  };

public:
  STAMINA_OBJECT_CLASS_VERSION(Transfer, Stamina::iSharedObject, Stamina::Version(0,1,0,0));

  Transfer(enType type, int net = 0, int cnt = 0) {
    Stamina::LockerCS locker(_locker);

    _ref++;
    LARGE_INTEGER li;
    QueryPerformanceCounter(&li);
    _id = ((_ref << 16) | (li.LowPart & 0xFFFF));

    _net = net;
    _type = type;
    _cnt = cnt;
  }

  inline UINT getID() {
    return _id;
  }

  inline void setName(const Stamina::StringRef &name) {
    Stamina::LockerCS locker(_locker);

    _transfer_name = name;
  }

  inline Stamina::String getName() {
    Stamina::LockerCS locker(_locker);

    return _transfer_name;
  }

  inline __int64 getSpeed() {
    Stamina::LockerCS locker(_locker);

    return _last_speed; 
  }

  inline void setSpeed(__int64 Bps) {
    Stamina::LockerCS locker(_locker);

    _last_speed = Bps;
  }

  inline Stamina::Time64 getBeginTime() {
    Stamina::LockerCS locker(_locker);

    return _begin_time;
  }

  inline void setBeginTime(const Stamina::Time64 &time) {
    Stamina::LockerCS locker(_locker);

    _begin_time = time;
  }

  inline Stamina::String getRootPath() {
    Stamina::LockerCS locker(_locker);

    return _root_path;
  }

  inline void setRootPath(const Stamina::StringRef &path) {
    Stamina::LockerCS locker(_locker);

    _root_path = path;
  }

  inline UINT addItem(Item* item) {
    Stamina::LockerCS locker(_locker);

    if (haveItem(item->getName(), item->getType())) return 0;
    _items.push_back(item);
    return item->getID();
  }

  inline Item* getItem(UINT id) {
    Stamina::LockerCS locker(_locker);

    tItem::iterator it = _items.begin();
    for (;it != this->_items.end(); it++) {
      if ((*it)->getID() == id) {
        return (*it);
      }
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
    if (haveItem(name, item->getType())) return 0;
    tItem::iterator it = _items.begin();
      for (;it != this->_items.end(); it++) {
        if (((*it)->getID() == id)) {
          return (*it)->setName(name);
        }
      }
    return 0;
  }
  
public:
  tItem _items;

private:
  UINT _id;
  static UINT _ref;

  int _cnt;
  int _net;
  enType _type;

  Stamina::Time64 _begin_time;
  Stamina::String _transfer_name;
  __int64 _last_speed;

  Stamina::String _root_path;

  Stamina::CriticalSection_w32 _locker;
};

UINT Transfer::_ref = 0;

#endif /*__TRANSFER_H__*/