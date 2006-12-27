#pragma once

#ifndef __FILE_H__
#define __FILE_H__

#include "stdafx.h"
#include "Item.h"
#include "Directory.h"
#include <vector>

namespace kTransfer3 {
  class File: public Item {
  public:
    File(const Stamina::StringRef &name = "", void* parent = NULL): Item(name) {
      _type = enType::typeFile;
      _parent = parent;
    }

    virtual inline bool isOpened() {}
    virtual inline bool close() {}
    virtual inline bool addData(DWORD offset, void *buff, DWORD size) {}
    virtual inline bool getData(DWORD offset, void *buff, DWORD size) {}

  protected:
    void* _parent;
  };

  typedef std::vector<File*> tFiles;
};

#endif /*__FILE_H__*/