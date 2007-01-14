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

#include <vector>

#include "Item.h"
#include "ItemCollection.h"

#include "File.h"
#include "Transfer.h"

namespace kTransfer3 {
  // Directory Separator
  static const char DS[] = "\\";

  class Directory: public ItemCollection, public Item {
  public:
    static const UINT type = 2;

  public:
    Directory(const StringRef& name = "", Transfer* transfer = NULL, Directory* parent_dir = NULL): 
      Item(type, name, transfer), ItemCollection(parent_dir) { }

    virtual inline void setState(enState state) {
      LockerCS locker(Item::_locker);

      Item::setState(state);
      setItemsState(state);
    }

    virtual String getPath() {
      if (getParent()) {
        return ((Directory*) getParent())->getPath() + DS + getName();
      } else {
        return getName();
      }
    }

    virtual Directory* addDirectory(const StringRef& name) {
      LockerCS locker(ItemCollection::_locker);

      Directory* dir = new Directory(name, getTransfer(), this);
      addItem(dir);
      return dir;
    }

    virtual File* addFile(const StringRef& name) {
      LockerCS locker(ItemCollection::_locker);

      File* file = new File(name, getTransfer(), this);
      addItem(file);
      return file;
    }

    virtual bool create() {
      if (exists()) return false;

      String path = getPath() + DS;
      UINT count = path.find(DS); // trzeba pomin¹c tworzenie glownego katalogu np. C:/

      while (path.size() > count + 1) {
        count += path.substr(count + 1).find(DS) + 1;
        CreateDirectory(path.substr(0, count + 1).a_str(), NULL);
      }
    }

    virtual inline bool exists() {
      DWORD code = GetFileAttributes(getPath().a_str());
      return (code != -1) && ((FILE_ATTRIBUTE_DIRECTORY ^ code) == 0);
    }
  };

  typedef std::vector<Directory*> tDirectories;
};

#endif /*__DIRECTORY_H__*/