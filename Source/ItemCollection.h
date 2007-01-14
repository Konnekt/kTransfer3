/**
  *  Transfer class - ItemCollection
  *
  *  Licensed under The GNU Lesser General Public License
  *  Redistributions of files must retain the above copyright notice.
  *
  *  @filesource
  *  @copyright    Copyright (c) 2006-2007 Ursus6
  *  @link         svn://konnekt.info/ktransfer3/ kTransfer3 plugin SVN Repo
  *  @version      $Revision: 29 $
  *  @modifiedby   $LastChangedBy: ursus6 $
  *  @lastmodified $Date: 2007-01-10 16:33:28 +0100 (Śr, 10 sty 2007) $
  *  @license      http://creativecommons.org/licenses/LGPL/2.1/
  */

#pragma once

#ifndef __ITEMCOLLECTION_H__
#define __ITEMCOLLECTION_H__

#include <vector>

#include "Item.h"
#include "Transfer.h"

namespace kTransfer3 {
  class eNoSuchItem: public ExceptionString {
  public:
    eNoSuchItem(const StringRef& name): ExceptionString("No item with name = " + name) { }
    eNoSuchItem(UINT id): ExceptionString("No item with id = " + inttostr(id)) { }
  };

  class eItemExists: public ExceptionString {
  public:
    eItemExists(const StringRef& name): ExceptionString("Item with name = " + name + " already exists") { }
    eItemExists(UINT id): ExceptionString("Item with id = " + inttostr(id) + " already exists") { }
  };

  class ItemCollection: public iObject {
  public:
    STAMINA_OBJECT_CLASS_VERSION(ItemCollection, iObject, Version(0,1,0,0));

  public:
    ItemCollection(ItemCollection* parent = NULL): _parent(parent) { }

    virtual ~ItemCollection() {
      for (tItems::iterator it = _items.begin(); it != _items.end(); it++) {
        delete *it;
      }
    }

    virtual ItemCollection* getParent() const {
      LockerCS locker(_locker);
      return _parent;
    }

    virtual void setParent(ItemCollection* parent) {
      LockerCS locker(_locker);
      _parent = parent;
    }

    virtual inline bool hasItem(UINT id) {
      try {
        getItem(id);
      } catch(...) {
        return false;
      }
      return true;
    }

    virtual inline bool hasItem(const StringRef& name, UINT type) {
      try {
        getItem(name, type);
      } catch(...) {
        return false;
      }
      return true;
    }

    virtual inline Item* getItem(UINT id) {
      LockerCS locker(_locker);

      for (tItems::iterator it = _items.begin(); it != _items.end(); it++) {
        if ((*it)->getID() == id) return *it;
      }
      throw eNoSuchItem(id);
    }

    virtual inline Item* getItem(const StringRef& name, UINT type) {
      LockerCS locker(_locker);

      for (tItems::iterator it = _items.begin(); it != _items.end(); it++) {
        if ((*it)->getType() == type && (*it)->getName() == name) return *it;
      }
      throw eNoSuchItem(name);
    }

    virtual inline tItems getItems(UINT type) {
      LockerCS locker(_locker);
      tItems items;

      for (tItems::iterator it = _items.begin(); it != _items.end(); it++) {
        if ((*it)->getType() == type) items.push_back(*it);
      }
      return items;
    }

    virtual inline void addItem(Item* item) {
      LockerCS locker(_locker);

      if (hasItem(item->getName(), item->getType())) {
        throw eItemExists(item->getName());
      }
      _items.push_back(item);
    }

    virtual inline void removeItem(UINT id) {
      LockerCS locker(_locker);

      for (tItems::iterator it = _items.begin(); it != _items.end(); it++) {
        if ((*it)->getID() == id) {
          _items.erase(it); return;
        }
      }
      throw eNoSuchItem(id);
    }

    virtual inline UINT getItemsCount() {
      LockerCS locker(_locker);
      return _items.size();
    }

    virtual inline Item* getItemByPos(UINT pos) {
      LockerCS locker(_locker);

      if (pos >= getItemsCount()) {
        throw eNoSuchItem(pos);
      }
      return _items[pos];
    }

    virtual inline void setItemsState(Item::enState state) {
      LockerCS locker(_locker);

      for (tItems::iterator it = _items.begin(); it != _items.end(); it++) {
        (*it)->setState(state);
      }
    }

  protected:
    CriticalSection _locker;
    ItemCollection* _parent;
    tItems _items;
  };
};

#endif /* __ITEMCOLLECTION_H__ */