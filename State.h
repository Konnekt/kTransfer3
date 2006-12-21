#ifndef __STATE_H__
#define __STATE_H__

class State {
public:
  enum state {
    stNone,
    stOffline,
    stActive,
    stPaused,
    stCompleted,
    stTransfered,
    stStopped,
    stStarted,
    stError,
    stAborted,
    stIgnored
  };

public:
  State() {
    _state = stNone;
  }
  inline state getState() {
    return _state;
  }
  bool setState(state st) {
    _state = st;
  }

protected:
  state _state;
};

#endif /*__STATE_H__*/