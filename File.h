#ifndef __FILE_H__
#define __FILE_H__

#include "stdafx.h"
#include <vector>

typedef std::vector<class File *> tFiles;

class File {
public:
  inline bool isOpened() {}
  inline bool close() {}
  inline bool addData(DWORD offset, void *buff, DWORD size) {}
  inline bool getData(DWORD offset, void *buff, DWORD size) {}

private:
protected:

};
#endif /*__FILE_H__*/