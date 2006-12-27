#pragma once

#ifndef __QUEUE_H__
#define __QUEUE_H__

#include <vector>

class Queue {
public:
  typedef std::vector<oTransfer> tTransfers;

public:
  Queue() {}

  void refreshTransfer(int id) {}

  inline void Queue::insertTransfer(const oTransfer &transfer) {
    Stamina::LockerCS locker(_locker);

    _queue.push_back(transfer);
  }

  inline bool Queue::removeTransfer(UINT id) {
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

  inline oTransfer Queue::getTransfer(UINT id) {
    Stamina::LockerCS locker(_locker);

    tTransfers::iterator it = _queue.begin();
    for (;it != _queue.end(); it++) {
      if ((*it)->getID() == id) {
        return (*it);
      }
    }
    return NULL;
  }

  inline bool Queue::haveTransfer(UINT id) {
    Stamina::LockerCS locker(_locker);

    tTransfers::iterator it = _queue.begin();
    for (;it != _queue.end(); it++) {
      if ((*it)->getID() == id) return true;
    }
    return false;
  }

protected:
  Stamina::CriticalSection _locker;
  tTransfers _queue;
};

#endif /*__QUEUE_H__*/
