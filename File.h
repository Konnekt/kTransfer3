#ifndef __FILE_H__
#define __FILE_H__

#include "stdafx.h"
#include "Directory.h"
#include "Item.h"
#include <vector>

typedef std::vector<class File *> tFiles;

class File: public Item {
public:
  File(void* parent = NULL) {
    _parent = parent;
  }
  inline void setName(const Stamina::StringRef &name) {
    _name = name;
  }
  inline Stamina::String getName() {
    return _name;
  }
  inline bool isOpened() {}
  inline bool close() {}
  inline bool addData(DWORD offset, void *buff, DWORD size) {}
  inline bool getData(DWORD offset, void *buff, DWORD size) {}

protected:
  void* _parent;
  Stamina::String _name;
};
#endif /*__FILE_H__*/