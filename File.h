#pragma once

#ifndef __FILE_H__
#define __FILE_H__

#include "Item.h"
#include "Directory.h"
#include <vector>

class File: public Item {
public:
  File(const Stamina::StringRef &name = "", Directory* parent = NULL): Item(name) {
    _type = enType::typeFile;
    _parent = parent;
  }

  virtual inline bool isOpened() {}
  virtual inline bool close() {}
  virtual inline bool addData(DWORD offset, void *buff, DWORD size) {}
  virtual inline bool getData(DWORD offset, void *buff, DWORD size) {}

protected:
  Directory* _parent;
};

typedef std::vector<File*> tFiles;

#endif /*__FILE_H__*/