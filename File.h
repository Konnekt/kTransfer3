#ifndef __FILE_H__
#define __FILE_H__

#include "stdafx.h"
#include "Item.h"
#include "Directory.h"
#include <vector>

class File;
typedef std::vector<File *> tFiles;

class File: public Item {
public:
  File(void* parent = NULL) {
    _type = enType::tFile;
    _parent = parent;
  }

  virtual inline bool isOpened() {}
  virtual inline bool close() {}
  virtual inline bool addData(DWORD offset, void *buff, DWORD size) {}
  virtual inline bool getData(DWORD offset, void *buff, DWORD size) {}

protected:
  void* _parent;
};
#endif /*__FILE_H__*/