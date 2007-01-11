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
    typedef std::map<int, sObserver*> tEventObservers;
    typedef std::map<UINT, tEventObservers> tObservers;

  public:
    Event() {
      _ref = 0;
    }

    UINT registerObserver(Item* item, fOnEvent f, int event_, int priority) {
      if (++_ref == 0) _ref++;
      if (_observers[item->getID()].find(event_) == _observers[item->getID()].end()) {
         _observers[item->getID()][event_] = new sObserver;
      }
      return ((_observers[item->getID()][event_]->connections[_ref] = _observers[item->getID()][event_]->sig.connect(priority, f)).connected()) ? _ref : 0;
    }

    bool deleteObserver(Item* item, int event_, UINT id) {
      if (_observers.find(item->getID()) == _observers.end()) return false;
      if (_observers[item->getID()].find(event_) != _observers[item->getID()].end()) return false;
      if (_observers[item->getID()][event_]->connections.find(id) == _observers[item->getID()][event_]->connections.end()) return false;
      _observers[item->getID()][event_]->connections[id].disconnect();
      return true;
    }

    bool deleteAllObservers(Item* item) {
      if (_observers.find(item->getID()) == _observers.end()) return false;
      
      tEventObservers::iterator ite = _observers[item->getID()].begin();
      for(;ite != _observers[item->getID()].end(); ite++) {
        tConnections::iterator itc = ite->second->connections.begin();
        for(;itc != ite->second->connections.end(); itc++) {
          itc->second.disconnect();
        }
        delete ite->second;
        _observers[item->getID()].erase(ite);
      }
      return true;
    }

    bool sendEvent(Item* item, int event_) {
      if (_observers.find(item->getID()) == _observers.end()) return false;
      if (_observers[item->getID()].find(event_) != _observers[item->getID()].end()) return false;
      _observers[item->getID()][event_]->sig(item, event_);
      return true;
    }
   
  private:
    tObservers _observers;
    UINT _ref;

  };
};

#endif /*__EVENT_H__*/