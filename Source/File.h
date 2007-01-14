/**
  *  Transfer class - File
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

#ifndef __FILE_H__
#define __FILE_H__

#include "Item.h"
#include "Directory.h"
#include <windows.h>
#include <vector>

namespace kTransfer3 {
  class File: public Item {
  public:
    static const UINT type = 1;

  public:
    File(const StringRef& name = "", Transfer* transfer = NULL, Directory* parent_dir = NULL): 
      Item(type, name, transfer), _parent_dir(parent_dir), _opened(false) { }

    virtual ~File() {
      close();
    }

    virtual inline bool isOpened() {
      LockerCS locker(_locker);
      return _opened;
    }

    virtual bool open(bool overwrite) {
      if (isOpened()) return false;
      LockerCS locker(_locker);

      HANDLE hFile = CreateFile(getPath().a_str(), GENERIC_WRITE, 0, NULL, (overwrite) ? TRUNCATE_EXISTING : OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
      if (hFile == INVALID_HANDLE_VALUE) {
        return (overwrite) ? open(false) : false;
      }
      _handle = hFile;
      _opened = true;

      return true;
    }

    virtual inline bool remove() {
      LockerCS locker(_locker);
      return DeleteFile(getPath().a_str());
    }

    virtual inline bool exists() {
      LockerCS locker(_locker);
      DWORD code = GetFileAttributes(getPath().a_str());
      return (code != -1) && ((FILE_ATTRIBUTE_DIRECTORY & code) == 0);
    }

    virtual inline bool close() {
      if (!isOpened()) return false;
      LockerCS locker(_locker);

      _opened = false;
      return CloseHandle(_handle);
    }

    virtual bool write(DWORD offset, void* buffer, DWORD size) {
      LockerCS locker(_locker);
	    DWORD bytes_written;

      if ((size == 0) || !isOpened() || !buffer || !setSeek(offset)) return 0;
	    if (!WriteFile(_handle, buffer, size, &bytes_written, NULL)) return 0;	

      return bytes_written;
    }

    virtual bool read(DWORD offset, void* buffer, DWORD size) {
      LockerCS locker(_locker);
      DWORD bytes_read;

      if ((size == 0) || !isOpened() || !buffer || !setSeek(offset)) return 0;
	    if (!ReadFile(_handle, buffer, size, &bytes_read, NULL) || !bytes_read) return 0;

      return bytes_read;
    }

    virtual bool setSeek(DWORD offset) const {
	    DWORD dwptr = SetFilePointer(_handle, offset, NULL, FILE_BEGIN);
      return (dwptr == 0xFFFFFF) ? false : true;
    }

    virtual DWORD getSize() {
      if (!isOpened()) return 0;
      LockerCS locker(_locker);

      DWORD size = GetFileSize(_handle, NULL);
      return (size == INVALID_FILE_SIZE) ? 0 : size;
    }

    virtual String getPath();

  protected:
    Directory* _parent_dir;
    HANDLE _handle;
    bool _opened;
  };

  typedef std::vector<File*> tFiles;
};

#endif /*__FILE_H__*/