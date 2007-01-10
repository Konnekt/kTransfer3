/**
  *  Transfer class - Transfer
  *
  *  Licensed under The GNU Lesser General Public License
  *  Redistributions of files must retain the above copyright notice.
  *
  *  @filesource
  *  @copyright    Copyright (c) 2006-2007 Ursus6
  *  @link         svn://konnekt.info/ktransfer3/ kTransfer3 plugin SVN Repo
  *  @version      $Revision$
  *  @modifiedby   $LastChangedBy$
  *  @lastmodified $Date$
  *  @license      http://creativecommons.org/licenses/LGPL/2.1/
  */

#pragma once

#ifndef __TRANSFER_H__
#define __TRANSFER_H__

#include "Item.h"
#include "Group.h"

namespace kTransfer3 {
  class Transfer: public Group, public SharedObject<iSharedObject> {
  public:
    enum enType {
      typeNone = 0,
      typeFile,
      typeFiles,
      typeFilesFolders,
      typeImage,
      typeOther
    };

  public:
    STAMINA_OBJECT_CLASS_VERSION(Transfer, iSharedObject, Version(0,1,0,0));

    Transfer(enType type, int net = 0, int cnt = 0) {
      LockerCS locker(_locker);

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

    inline void setName(const StringRef &name) {
      LockerCS locker(_locker);

      _name = name;
    }
    inline String getName() {
      LockerCS locker(_locker);

      return _name;
    }

    inline __int64 getSpeed() {
      LockerCS locker(_locker);

      return _last_speed; 
    }
    inline void setSpeed(__int64 bps) {
      LockerCS locker(_locker);

      _last_speed = bps;
    }

    inline Time64 getBeginTime() {
      LockerCS locker(_locker);

      return _begin_time;
    }
    inline void setBeginTime(const Time64 &time) {
      LockerCS locker(_locker);

      _begin_time = time;
    }

     virtual inline String getSavePath() {
      LockerCS locker(_locker);

      return _save_path;
    }
    inline void setSavePath(const StringRef &path) {
      LockerCS locker(_locker);

      _save_path = path;
    }

    inline bool addItem(Item* item) {
      LockerCS locker(_locker);

      if (haveItem(item->getName(), item->getType())) {
        return false;
      }
      _items.push_back(item);
      return true;
    }

    inline Item* getItem(UINT id) {
      LockerCS locker(_locker);

      tItems::iterator it = _items.begin();
      for (;it != this->_items.end(); it++) {
        if ((*it)->getID() == id) return (*it);
      }
      return NULL;
    }

    inline bool removeItem(UINT id) {
      LockerCS locker(_locker);

      tItems::iterator it = _items.begin();
      for (;it != this->_items.end(); it++) {
        if ((*it)->getID() == id) {
          delete (*it);
          _items.erase(it); 
          return true;
        }
      }
      return false;
    }

    inline UINT getItemID(const StringRef &name, UINT type) {
      LockerCS locker(_locker);

      tItems::iterator it = _items.begin();
        for (;it != this->_items.end(); it++) {
          if (((*it)->getName() == name) && ((*it)->getType() == type)) {
            return (*it)->getID();
          }
        }
      return 0;
    }

    inline bool haveItem(const StringRef &name, UINT type) {
      LockerCS locker(_locker);

      return getItemID(name, type);
    }

    virtual inline UINT getItemsCount() {
      LockerCS locker(_locker);

      return _items.size();
    }

    virtual inline Item* getItemT(UINT id_t) {
      LockerCS locker(_locker);

      if (id_t >= getItemsCount()) return NULL;
      return _items[id_t];
    }

    inline bool setItemName(UINT id, const StringRef &name) {
      LockerCS locker(_locker);

      Item *item = getItem(id);
      if (haveItem(name, item->getType())) {
        return false;
      }
      tItems::iterator it = _items.begin();
        for (;it != this->_items.end(); it++) {
          if (((*it)->getID() == id)) {
            (*it)->setName(name);
            return true;
          }
        }
      return false;
    }
    
  public:
    tItems _items;

  private:
    static UINT _ref;
    UINT _id;

  protected:
    CriticalSection _locker;
    __int64 _last_speed;
    enType _type;

    int _cnt;
    int _net;

    Time64 _begin_time;
    String _save_path;
    String _name;
  };

  typedef SharedPtr<Transfer> oTransfer;

  UINT Transfer::_ref = 0;
};

#endif /*__TRANSFER_H__*/