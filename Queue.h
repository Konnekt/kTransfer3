#pragma once

#ifndef __QUEUE_H__
#define __QUEUE_H__

#include <vector>

namespace kTransfer3 {
  class Queue {
  public:
    typedef std::vector<oTransfer> tTransfers;

  public:
    Queue() {}

    void refreshTransfer(UINT id) {}

    inline void insertTransfer(const oTransfer &transfer) {
      Stamina::LockerCS locker(_locker);

      _queue.push_back(transfer);
    }

    inline bool removeTransfer(UINT id) {
      Stamina::LockerCS locker(_locker);

      tTransfers::iterator it = _queue.begin();
      for (;it != _queue.end(); it++) {
        if ((*it)->getID() == id) {
          _queue.erase(it);       
          return true;
        }
      }
      return false;
    }

    inline oTransfer getTransfer(UINT id) {
      Stamina::LockerCS locker(_locker);

      tTransfers::iterator it = _queue.begin();
      for (;it != _queue.end(); it++) {
        if ((*it)->getID() == id) {
          return (*it);
        }
      }
      return NULL;
    }

    inline oTransfer getTransferT(UINT id_t) {
      Stamina::LockerCS locker(_locker);

      if (id_t >= countTransfer()) return NULL;
      return _queue[id_t];
    }

    inline bool haveTransfer(UINT id) {
      Stamina::LockerCS locker(_locker);

      tTransfers::iterator it = _queue.begin();
      for (;it != _queue.end(); it++) {
        if ((*it)->getID() == id) return true;
      }
      return false;
    }

    inline UINT countTransfer() {
      return _queue.size();
    }

  protected:
    Stamina::CriticalSection _locker;
    tTransfers _queue;
  };
};

#endif /*__QUEUE_H__*/
