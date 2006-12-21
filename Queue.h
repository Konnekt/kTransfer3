#ifndef __QUEUE_H__
#define __QUEUE_H__

#include <vector>

using namespace std;

#include "Transfer.h"

class Queue {
public:
  typedef vector<oTransfer> tTransfers;

public:
  Queue() {};
  int insertTransfer(const oTransfer &transfer);
  int removeTransfer(int id);
  oTransfer getTransfer(int id);
  bool existTransfer(int id);
  void refreshTransfer(int id);

private:

protected:
  Stamina::CriticalSection_w32 _locker;
  tTransfers _queue;
};

#endif /*__QUEUE_H__*/