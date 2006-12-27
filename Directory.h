#pragma once

#ifndef __DIRECTORY_H__
#define __DIRECTORY_H__

#include "Item.h"
#include "File.h"
#include <vector>

class Directory: public Item {
public:
  Directory(const Stamina::StringRef &name = "", Directory* parent = NULL): Item(name) {
    _type = enType::typeDirectory;
    _parent = parent;
  }
  ~Directory() {
    tDirectories::iterator it = _directories.begin();
    for (;it != this->_directories.end(); it++) {
      delete *it;
    }
    tFiles::iterator it = _files.begin();
    for (;it != _files.end(); it++) {
      delete *it;
    }
  }
  inline Directory* getParent() {
    Stamina::LockerCS locker(_locker);

    return _parent;
  }

  inline bool setDirectoryName(UINT id, const Stamina::StringRef &name) {
    Stamina::LockerCS locker(_locker);

    if (haveDirectory(name)) return false;
    tDirectories::iterator it = _directories.begin();
    for (;it != _directories.end(); it++) {
      if ((*it)->getID() == id) {
        return (*it)->setName(name);
      }
    }
    return true;
  }

  virtual inline int getDirectoryID(const Stamina::StringRef &name) {
    Stamina::LockerCS locker(_locker);

    tDirectories::iterator it = _directories.begin();
    for (;it != _directories.end(); it++) {
      if ((*it)->getName() == name) {
        return (*it)->getID();
      }
    }
    return 0;
  }

  virtual bool haveDirectory(const Stamina::StringRef &name) {
    Stamina::LockerCS locker(_locker);

    return getDirectoryID(name);
  }

  virtual inline int addDirectory(const Stamina::StringRef &name) {
    Stamina::LockerCS locker(_locker);

    Directory* directory = new Directory(name, this);
    _directories.push_back(directory);
    return directory->getID();
  }

  virtual inline Directory* getDirectory(const Stamina::StringRef &name) {
    Stamina::LockerCS locker(_locker);

    tDirectories::iterator it = _directories.begin();
    for (;it != _directories.end(); it++) {
      if ((*it)->getName() == name) {
        return (*it);
      }
    }
    return NULL;
  }

  virtual inline bool removeDirectory(UINT id) {
    Stamina::LockerCS locker(_locker);

    tDirectories::iterator it = _directories.begin();
    for (;it != _directories.end(); it++) {
      if ((*it)->getID() == id) {
        delete (*it);
        _directories.erase(it); 
        return true;
      }
    }
    return false;
  }

  virtual inline UINT getDirectoryCount() {
    Stamina::LockerCS locker(_locker);

    return _directories.size();
  }

  virtual inline Directory* getDirectoryT(UINT id_t) {
    Stamina::LockerCS locker(_locker);

    return _directories[id_t];
  }

  virtual inline UINT addFile(const Stamina::StringRef &name) {
    Stamina::LockerCS locker(_locker);

    File* file = new File(name, this)
    _files.push_back(file);
    return file->getID();
  }

  inline bool removeFile(UINT id) {
    Stamina::LockerCS locker(_locker);

    tFiles::iterator it = _files.begin();
    for (;it != _files.end(); it++) {
      if ((*it)->getID() == id ) {
        delete (*it);
        _files.erase(it); 
        return true;
      }
    }
    return false;
  }

  inline File* getFile(UINT id) {
    Stamina::LockerCS locker(_locker);

    tFiles::iterator it = _files.begin();
    for (;it != _files.end(); it++) {
      if ((*it)->getID() == id) {
        return (*it);
      }
    }
    return NULL;
  }

  inline File* getFile(const Stamina::StringRef &name) {
    Stamina::LockerCS locker(_locker);

    tFiles::iterator it = _files.begin();
    for (;it != _files.end(); it++) {
      if ((*it)->getName() == name) {
        return (*it);
      }
    }
    return NULL;
  }

  inline UINT getFileID(const Stamina::StringRef &name) {
    Stamina::LockerCS locker(_locker);

    tFiles::iterator it = _files.begin();
    for (;it != _files.end(); it++) {
      if ((*it)->getName() == name) {
        return (*it)->getID();
      }
    }
    return 0;
  }

  virtual inline UINT getFileCount() {
    Stamina::LockerCS locker(_locker);

    return _files.size();
  }

  virtual inline File* getFileT(UINT id_t) {
    Stamina::LockerCS locker(_locker);

    return _files[id_t];
  }

  virtual inline bool haveFile(const Stamina::StringRef &name) {
    Stamina::LockerCS locker(_locker);

    return getFileID(name);
  }

  virtual inline bool setFileName(UINT id, const Stamina::StringRef &name) {
    Stamina::LockerCS locker(_locker);

    if (haveFile(name)) return false;
    tFiles::iterator it = _files.begin();
    for (;it != _files.end(); it++) {
      if ((*it)->getID() == id ) {
        return (*it)->setName(name);
      }
    }
    return true;
  }

  virtual inline void setDirectoriesState(enState state) {
    Stamina::LockerCS locker(_locker);

    tDirectories::iterator it = _directories.begin();
    for (;it != _directories.end(); it++) {
      (*it)->setState(state);
    }
  }

  virtual inline void setFilesState(enState state) {
    Stamina::LockerCS locker(_locker);

    tFiles::iterator it = _files.begin();
    for (;it != _files.end(); it++) {
      (*it)->setState(state);
    }
  }

  virtual inline void setState(enState state) {
    Stamina::LockerCS locker(_locker);

    setDirectoriesState(state);
    setFilesState(state);
    Item::setState(state);
  }

protected:
  Directory* _parent;
  tDirectories _directories;
  tFiles _files;
};

typedef std::vector<Directory*> tDirectories;

#endif /*__DIRECTORY_H__*/