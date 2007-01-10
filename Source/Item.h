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
      stIgnored,
      stOK
   };

  public:
    Item(UINT type, const StringRef &name = "", Transfer* parent = NULL) {
      LockerCS locker(_locker);

      // generacja losowego id
      _ref++;
      LARGE_INTEGER li;
      QueryPerformanceCounter(&li);
      // ref z li.LowPart powinny stworzy� naprawd� unikalny id
      _id = (_ref << 16) | (li.LowPart & 0xFFFF);

      // od tej chwili domysln� nazw� itemu jest jego id, zapisany numerycznie
      _name = !name.length() ? inttostr(_id) : name;

      _parent = parent;
      _type = type;
    }

    virtual inline UINT getID() const {
      return _id;
    }

    virtual inline enState getState() {
      LockerCS locker(_locker);

      return _state;
    }

    virtual inline void setState(enState state) {
      LockerCS locker(_locker);

      _state = state;
    }

    virtual inline UINT getType() const {
      return _type;
    }

    virtual inline void setName(const StringRef &name) {
      LockerCS locker(_locker);

      _name = name;
    }

    virtual inline String getName() {
      LockerCS locker(_locker);

      return _name;
    }

    virtual inline Transfer* getParent() const {
      return _parent;
    }



  private:
    static UINT _ref;
    UINT _id;

  protected:
    Transfer *_parent;
    String _name;
    enState _state;
    UINT _type;

    CriticalSection _locker;
  };

  UINT Item::_ref = 0;
};

#endif /*__ITEM_H__*/