#ifndef __GROUP_H__
#define __GROUP_H__

class Group {
public:
  enum enGroup{
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
  inline bool setGroup(enGroup group) {
    if (_group != grNone) {
      return false;
    }
    _group = group;
    return true;
  }
  inline enGroup getGroup() {
    return _group;
  }

private:
  enGroup _group;
};

#endif /*__GROUP_H__*/