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
  inline bool setName(const Stamina::StringRef &name) {
    if (getDirectoryID(name) != -1) {
      return false;
    }
    _name = name;
    return true;
  }
  inline Stamina::String getName() {
    return _name;
  }
  inline bool addDirectory(const Stamina::StringRef &name) {
    if (getDirectoryID(name) != -1) {
      return false;
    }
    Directory* directory = new Directory(this);
    if (directory == NULL) {
      return false;
    }
    directory->setName(name);
    _directories.push_back(directory);
    return true;
  }
  inline Directory* getDirectory(int id) {
    tDirectories::iterator it = _directories.begin();
    for (;it != this->_directories.end(); it++) {
      if ((*it)->getID() == id ) {
        return (*it);
      }
    }
    return NULL;
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
  inline int getDirectoryID(const Stamina::StringRef &name) {
    tDirectories::iterator it = _directories.begin();
    for (;it != this->_directories.end(); it++) {
      if ((*it)->getName() == name ) {
        return (*it)->getID();
      }
    }
    return -1;
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
  /*
  inline bool addFile(const Stamina::StringRef &name) {
    if (getFileID(name) != -1) {
      return false;
    }
    File* file = new File();
    if (file == NULL) {
      return false;
    file->setName(name);
    _files.push_back(file);
    return true;
  }
  inline bool removeFile(int id) {
    File *file = _files[id];
    if (file == NULL) {
      return false;
    }
    delete file;
    tFiles::iterator it = _files.begin() + id;
    _files.erase(it); 
    return true;
  }
  inline File* getFile(int id) {
    return _files[id];
  }
  inline File* getFile(const Stamina::StringRef &name) {
    tDirectories::iterator it = _files.begin();
    for (;it != this->_files.end(); it++) {
      if ((*it)->getName() == name ) {
        return (*it);
      }
    }
    return NULL;
  }
  inline int getFileID(const Stamina::StringRef &name) {
    tDirectories::iterator it = _files.begin();
    for (;it != this->_files.end(); it++) {
      if ((*it)->getName() == name ) {
        return it - _files.begin();
      }
    }
    return -1;
  }*/
  //TODO sprawdzenie jednej nazwy dla 1 pliku

private:
  Stamina::String _name;
  Directory* _parent;

protected:
  tDirectories _directories;
  tFiles _files;
};

#endif /*__DIRECTORY_H__*/