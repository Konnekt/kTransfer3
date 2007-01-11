#ifndef __EVENT_H__
#define __EVENT_H__

#include <map>
#include <boost/signal.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include "Item.h"

namespace kTransfer3 {
  class Event {
  public: 
    typedef function<void(Item*, int event_)> fOnEvent;
    typedef signal<void(Item*, int event_)> sigOnEvent;
    typedef std::map<UINT, signals::connection> tConnections;

    struct sObserver {
      tConnections connections;
      sigOnEvent sig;
    };

    typedef std::map<Item*, std::map<int, sObserver*>> tObservers;

  public:
    Event() {
      _ref = 0;
    }

    UINT registerObserver(Item* item, fOnEvent f, int event_, int priority) {
      if (++_ref == 0) _ref++;
      if (_observers[item].find(event_) == _observers[item].end()) {
         _observers[item][event_] = new sObserver;
      }
      return ((_observers[item][event_]->connections[_ref] = _observers[item][event_]->sig.connect(priority, f)).connected()) ? _ref : 0;
    }

    bool deleteObserver(Item* item, int event_, UINT id) {
      if (_observers[item].find(event_) != _observers[item].end()) return false;
      if (_observers[item][event_]->connections.find(id) == _observers[item][event_]->connections.end()) return false;
      _observers[item][event_]->connections[id].disconnect();
      return true;
    }

    void sendEvent(Item* item, int event_) {
      _observers[item][event_]->sig(item, event_);
    }
   
  private:
    tObservers _observers;
    UINT _ref;

  };
};

#endif /*__EVENT_H__*/