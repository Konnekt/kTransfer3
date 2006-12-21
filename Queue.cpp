#include "stdafx.h"
#include "Queue.h"

int Queue::insertTransfer(const oTransfer &transfer) {
  Stamina::LockerCS locker(_locker);
  this->_queue.push_back(transfer);
  return this->_queue[this->_queue.size() - 1]->getID();
}

int Queue::removeTransfer(int id) {
  Stamina::LockerCS locker(_locker);
  tTransfers::iterator it = this->_queue.begin();
  for (;it != this->_queue.end(); it++) {
    if ((*it)->getID() == id ) {
      _queue.erase(it);       
      return id;
    }
  }
  return 0;
}

oTransfer Queue::getTransfer(int id) {
  Stamina::LockerCS locker(_locker);
  tTransfers::iterator it = this->_queue.begin();
  for (;it != this->_queue.end(); it++) {
    if ((*it)->getID() == id ) {
      return (*it);
    }
  }
  return NULL;
}

bool Queue::existTransfer(int id) {
  Stamina::LockerCS locker(_locker);
  tTransfers::iterator it = this->_queue.begin();
  for (;it != this->_queue.end(); it++) {
    if ((*it)->getID() == id ) {
      return true;
    }
  }
  return false;
}










