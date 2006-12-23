#ifndef __DIRECTORY_H__
#define __DIRECTORY_H__

#include "stdafx.h"
#include "Item.h"
#include "File.h"
#include <vector>

typedef std::vector<class Directory *> tDirectories; 

class Directory: public Item {
public:
  Directory(Directory* parent = NULL) {
    _parent = parent;
  }
  inline Directory* getParent() {
    return _parent;
  }
  virtual inline bool setName(const Stamina::StringRef &name) {
    if (getDirectoryID(name) != 0) {
      return false;
    }
    _name = name;
    return true;
  }
  virtual inline int getDirectoryID(const Stamina::StringRef &name) {
    tDirectories::iterator it = _directories.begin();
    for (;it != this->_directories.end(); it++) {
      if ((*it)->getName() == name ) {
        return (*it)->getID();
      }
    }
    return 0;
  }
  virtual inline Stamina::String getName() {
    return _name;
  }

  virtual inline int addDirectory(const Stamina::StringRef &name) {
    if (getDirectoryID(name) != 0) {
      return 0;
    }
    Directory* directory = new Directory(this);
    if (directory == NULL) {
      return 0;
    }
    directory->setName(name);
    _directories.push_back(directory);
    return directory->getID();
  }

  inline Directory* getDirectory(const Stamina::StringRef &name) {
    tDirectories::iterator it = _directories.begin();
    for (;it != this->_directories.end(); it++) {
      if ((*it)->getName() == name ) {
        return (*it);
      }
    }
    return NULL;
  }
  inline bool removeDirectory(int id) {
    tDirectories::iterator it = _directories.begin();
    for (;it != this->_directories.end(); it++) {
      if ((*it)->getID() == id ) {
        delete (*it);
        _directories.erase(it); 
        return true;
      }
    }
    return false;
  }
  
  inline int addFile(const Stamina::StringRef &name) {
    if (getFileID(name) != 0) {
      return 0;
    }
    File* file = new File(this);
    if (file == NULL) {
      return 0;
    }
    file->setName(name);
    _files.push_back(file);
    return file->getID();
  }
  inline bool removeFile(int id) {
    tFiles::iterator it = _files.begin();
    for (;it != this->_files.end(); it++) {
      if ((*it)->getID() == id ) {
        delete (*it);
        _files.erase(it); 
        return true;
      }
    }
    return false;
  }

  inline File* getFile(int id) {
    tFiles::iterator it = _files.begin();
    for (;it != this->_files.end(); it++) {
      if ((*it)->getID() == id ) {
        return (*it);
      }
    }
    return NULL;
  }
  inline File* getFile(const Stamina::StringRef &name) {
    tFiles::iterator it = _files.begin();
    for (;it != this->_files.end(); it++) {
      if ((*it)->getName() == name ) {
        return (*it);
      }
    }
    return NULL;
  }
  inline int getFileID(const Stamina::StringRef &name) {
    tFiles::iterator it = _files.begin();
    for (;it != this->_files.end(); it++) {
      if ((*it)->getName() == name ) {
        return (*it)->getID();
      }
    }
    return 0;
  }
  inline void setDirectoriesState(enState state) {
    tDirectories::iterator it = _directories.begin();
    for (;it != _directories.end(); it++) {
      (*it)->setState(state);
    }
  }
  inline void setFilesState(enState state) {
    tFiles::iterator it = _files.begin();
    for (;it != _files.end(); it++) {
      (*it)->setState(state);
    }
  }
  inline void setState(enState state) {
    _state = state;
    setFilesState(state);
    setDirectoriesState(state);
  }

protected:
  Stamina::String _name;
  Directory* _parent;
  tDirectories _directories;
  tFiles _files;
};

#endif /*__DIRECTORY_H__*/