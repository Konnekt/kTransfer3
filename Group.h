#ifndef __GROUP_H__
#define __GROUP_H__

class Group {
public:
  enum group{
    grNone,
    grReceiving,
    grSending,
    grChecking,
    grOther
  };

public:
  Group() {
    _group = grNone;
  }
  inline bool setGroup(group grp) {
    if (_group != grNone) {
      return false;
    }
    _group = grp;
    return true;
  }
  inline group getGroup() {
    return _group;
  }

private:
  group _group;
};

#endif /*__GROUP_H__*/