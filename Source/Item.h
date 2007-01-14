/**
  *  Transfer class - Item
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

#ifndef __ITEM_H__
#define __ITEM_H__

#include <windows.h>
#include "Transfer.h"
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

  public:
    Item(UINT type, const StringRef& name = "", Transfer* transfer = NULL): _transfer(transfer), _type(type), _id(getRandomID()) {
      _name = !name.length() ? inttostr(_id) : name;
    }

    inline static UINT getRandomID() {
      // generacja losowego id
      LARGE_INTEGER li;
      QueryPerformanceCounter(&li);

      return (++_ref << 16) | (li.LowPart & 0xFFFF);
    }

    virtual inline UINT getID() const {
      return _id;
    }

    virtual inline UINT getType() const {
      return _type;
    }

    virtual inline enState getState() {
      LockerCS locker(_locker);
      return _state;
    }
    virtual inline void setState(enState state) {
      LockerCS locker(_locker);
      _state = state;
    }

    virtual inline String getName() {
      LockerCS locker(_locker);
      return _name;
    }
    virtual inline void setName(const StringRef &name) {
      LockerCS locker(_locker);
      _name = name;
    }

    virtual inline Transfer* getTransfer() {
      return _transfer;
    }

    virtual bool removeFromTransfer();

    inline void destroy() {
      removeFromTransfer();
      delete this;
    }

  private:
    static UINT _ref;
    UINT _id;

  protected:
    CriticalSection _locker;

    Transfer* _transfer;
    enState _state;
    String _name;
    UINT _type;
  };

  // item list
  typedef std::vector<Item*> tItems;

  // initialization
  UINT Item::_ref = 0;
};

#endif /*__ITEM_H__*/