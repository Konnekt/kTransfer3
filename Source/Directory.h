/**
  *  Transfer class - Directory
  *
  *  Licensed under The GNU Lesser General Public License
  *  Redistributions of files must retain the above copyright notice.
  *
  *  @filesource
  *  @copyright    Copyright (c) 2006-2007 Ursus6
  *  @link         svn://konnekt.info/ktransfer3/ kTransfer3 plugin SVN Repo
  *  @version      $Revision$
  *  @modifiedby   $LastChangedBy$
  *  @lastmodified $Date$
  *  @license      http://creativecommons.org/licenses/LGPL/2.1/
  */

#pragma once

#ifndef __DIRECTORY_H__
#define __DIRECTORY_H__

#include "Transfer.h"
#include "Item.h"
#include "File.h"
#include <vector>

namespace kTransfer3 {
  typedef std::vector<class Directory*> tDirectories;

  class Directory: public Item {
  public:
    Directory(const StringRef &name = "", Transfer* parent = NULL, Directory* parent_dir = NULL): Item(30, name, parent) {
      _parent_dir = parent_dir;
    }

    ~Directory() {
      tDirectories::iterator it = _directories.begin();
      for (;it != this->_directories.end(); it++) {
        delete *it;
      }
      tFiles::iterator itf = _files.begin();
      for (;itf != _files.end(); itf++) {
        delete *itf;
      }
    }

    virtual void create() {
      if (isExists()) return ;
      String path = getPath() + "\\";
      unsigned int count = path.find("\\"); // trzeba pomin¹c tworzenie glownego katalogu np. C:/
      while (path.size() > count + 1) {
        count+= path.substr(count + 1).find("\\") + 1;
        CreateDirectory(path.substr(0, count + 1).a_str(), NULL);
      }
    }

    virtual Directory* getDirectoryParent() const {
      return _parent_dir;
    }

    virtual String getPath() {
      if (getDirectoryParent()) {
        return getDirectoryParent()->getPath() + "\\" + getName();
      }
      else {
        return getParent()->getName();
      }
    }

    virtual inline bool isExists() {
      DWORD code = GetFileAttributes(getPath().a_str());
      return (code != -1) && ((FILE_ATTRIBUTE_DIRECTORY ^ code) == 0);
    }

    virtual inline bool setDirectoryName(UINT id, const StringRef &name) {
      LockerCS locker(_locker);

      if (haveDirectory(name)) return false;
      tDirectories::iterator it = _directories.begin();
      for (;it != _directories.end(); it++) {
        if ((*it)->getID() == id) {
          (*it)->setName(name);
          return true;
        }
      }
      return true;
    }

    virtual inline UINT getDirectoryID(const StringRef &name) {
      LockerCS locker(_locker);

      tDirectories::iterator it = _directories.begin();
      for (;it != _directories.end(); it++) {
        if ((*it)->getName() == name) {
          return (*it)->getID();
        }
      }
      return 0;
    }

    virtual bool haveDirectory(const StringRef &name) {
      LockerCS locker(_locker);

      return getDirectoryID(name);
    }

    virtual inline UINT addDirectory(const StringRef &name) {
      LockerCS locker(_locker);

      Directory* directory = new Directory(name, _parent, this);
      _directories.push_back(directory);
      return directory->getID();
    }

    virtual inline Directory* getDirectory(const StringRef &name) {
      LockerCS locker(_locker);

      tDirectories::iterator it = _directories.begin();
      for (;it != _directories.end(); it++) {
        if ((*it)->getName() == name) {
          return (*it);
        }
      }
      return NULL;
    }

    virtual inline bool removeDirectory(UINT id) {
      LockerCS locker(_locker);

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
      LockerCS locker(_locker);

      return _directories.size();
    }

    virtual inline Directory* getDirectoryT(UINT id_t) {
      LockerCS locker(_locker);

      if (id_t >= getDirectoryCount()) return NULL;
      return _directories[id_t];
    }

    virtual inline UINT addFile(const StringRef &name) {
      LockerCS locker(_locker);

      File* file = new File(name, _parent, this);
      _files.push_back(file);
      return file->getID();
    }

    inline bool removeFile(UINT id) {
      LockerCS locker(_locker);

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
      LockerCS locker(_locker);

      tFiles::iterator it = _files.begin();
      for (;it != _files.end(); it++) {
        if ((*it)->getID() == id) {
          return (*it);
        }
      }
      return NULL;
    }

    inline File* getFile(const StringRef &name) {
      LockerCS locker(_locker);

      tFiles::iterator it = _files.begin();
      for (;it != _files.end(); it++) {
        if ((*it)->getName() == name) {
          return (*it);
        }
      }
      return NULL;
    }

    inline UINT getFileID(const StringRef &name) {
      LockerCS locker(_locker);

      tFiles::iterator it = _files.begin();
      for (;it != _files.end(); it++) {
        if ((*it)->getName() == name) {
          return (*it)->getID();
        }
      }
      return 0;
    }

    virtual inline UINT getFileCount() {
      LockerCS locker(_locker);

      return _files.size();
    }

    virtual inline File* getFileT(UINT id_t) {
      LockerCS locker(_locker);

      if(id_t >= getFileCount()) return NULL;
      return _files[id_t];
    }

    virtual inline bool haveFile(const StringRef &name) {
      LockerCS locker(_locker);

      return getFileID(name);
    }

    virtual inline bool setFileName(UINT id, const StringRef &name) {
      LockerCS locker(_locker);

      if (haveFile(name)) return false;
      tFiles::iterator it = _files.begin();
      for (;it != _files.end(); it++) {
        if ((*it)->getID() == id ) {
          (*it)->setName(name);
          return true;
        }
      }
      return true;
    }

    virtual inline void setDirectoriesState(enState state) {
      LockerCS locker(_locker);

      tDirectories::iterator it = _directories.begin();
      for (;it != _directories.end(); it++) {
        (*it)->setState(state);
      }
    }

    virtual inline void setFilesState(enState state) {
      LockerCS locker(_locker);

      tFiles::iterator it = _files.begin();
      for (;it != _files.end(); it++) {
        (*it)->setState(state);
      }
    }

    virtual inline void setState(enState state) {
      LockerCS locker(_locker);

      setDirectoriesState(state);
      setFilesState(state);
      setState(state);
    }

  private:
    Directory *_parent_dir;

  protected:
    tDirectories _directories;
    tFiles _files;
  };
};

#endif /*__DIRECTORY_H__*/