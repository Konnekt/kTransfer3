#ifndef __STATE_H__
#define __STATE_H__

class State {
public:
  enum enState {
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
  inline enState getState() {
    return _state;
  }
  bool setState(enState state) {
    _state = state;
  }

protected:
  enState _state;
};

#endif /*__STATE_H__*/