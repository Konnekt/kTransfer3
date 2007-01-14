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

#include "ItemCollection.h"
#include "Item.h"
#include "Group.h"

namespace kTransfer3 {
  class Transfer: public ItemCollection, public SharedObject<iSharedObject> {
  public:
    STAMINA_OBJECT_CLASS_VERSION(Transfer, iSharedObject, Version(0,1,0,0));

    Transfer(UINT type, tCntId cnt): _type(type), _cnt(cnt), _id(Item::getRandomID()) { }

    inline UINT getID() {
      return _id;
    }

    inline String getName() {
      LockerCS locker(_locker);
      return _name;
    }
    inline void setName(const StringRef &name) {
      LockerCS locker(_locker);
      _name = name;
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

  private:
    UINT _id;

  protected:
    CriticalSection _locker;

    UINT _type;
    tCntId _cnt;

    __int64 _last_speed;
    Time64 _begin_time;
    String _save_path;
    String _name;
  };

  typedef SharedPtr<Transfer> oTransfer;
};

#endif /*__TRANSFER_H__*/