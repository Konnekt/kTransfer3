#ifndef __TRANSFER_H__
#define __TRANSFER_H__

#include "State.h"
#include "Items.h"
#include "Group.h"
#include <vector>

typedef Stamina::SharedPtr<class Transfer> oTransfer;

class Transfer: public Group, public State, public Stamina::SharedObject<Stamina::iSharedObject> {
public:
  enum type {
    tNone,
    tOneFile,
    tMultiFile,
    tMultiFileFolders,
    tImage,
    tOther
  };

public:
  STAMINA_OBJECT_CLASS_VERSION(Transfer, Stamina::iSharedObject, Stamina::Version(0,1,0,0));

  Transfer(type t, int id, int net = 0, int cnt = 0) {
    _id = id;
    _net = net;
    _type = t;
    _cnt = cnt;
  }
  inline void setName(const Stamina::StringRef &name) {
    _transfer_name = name;
  }
  inline Stamina::String getName() {
    return _transfer_name;
  }
  inline int getID() {
    return _id;
  }
  inline __int64 getSpeed() {
    return _last_speed; 
  }
  inline void setSpeed(__int64 Bps) {
    _last_speed = Bps;
  }
  inline Stamina::Time64 getBeginTime() {
    return _begin_time;
  }
  inline void setBeginTime(const Stamina::Time64 &time) {
    _begin_time = time;
  }

private:
    int _id;
    int _cnt;
    int _net;
    type _type;

    Stamina::Time64 _begin_time;
    Stamina::String _transfer_name;
    __int64 _last_speed;
    type _transfer_type;

    Items items;
};

#endif /*__TRANSFER_H__*/