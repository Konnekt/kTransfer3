#include <map>
#include <boost/signal.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>

class Event {
public: 
  typedef function<void(Item*, int event_)> fOnEvent;
  typedef signal<void(Item*, int event_)> sigOnEvent;
  typedef std::map<Item*, sigOnEvent> tObservers;

  void registerObserver(Item*, fOnEvent f, int event_, int piority);
  void deleteObserver(Item*, int event_);
  void sendEvent(Item*, int event_);
};