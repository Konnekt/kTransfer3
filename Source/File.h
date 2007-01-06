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
#include <windows.h>
#include <vector>

namespace kTransfer3 {
  typedef std::vector<File*> tFiles;

  class File: public Item {
  public:
    File(const StringRef &name = "", Directory* parent = NULL): Item(name, (Item*)parent) {
      _type = enType::typeFile;
      _opened = false;
      _temp_name = "temp_" + name + /*temp_id +*/ ".part"; // domyslna wartosc
    }

    virtual ~File() {
      close();
    }

    virtual inline String getTempName() {
      return _temp_name;
    }

    virtual bool open(bool overwritte) {
      Stamina::LockerCS locker(_locker);
      if (isOpened()) return false;
      HANDLE hFile = CreateFile(getPath().a_str(), GENERIC_WRITE, 0, NULL, (overwritte) ? TRUNCATE_EXISTING : OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
      if (hFile == INVALID_HANDLE_VALUE) {
        if (overwritte) return open(false);
        else return false;
      }
      _file = hFile;
      _opened = true;  
      _temp_opened = false;
      return true;
    }

    virtual String getTempPath() {
      Stamina::LockerCS locker(_locker);
      Stamina::String path = getPath();
      path.erase(path.findLast("\\"));
      path+= "\\" + getTempName();
      return path;
    }

    virtual bool openTemp(bool overwritte) {
      Stamina::LockerCS locker(_locker);
      if (isOpened()) return false;
      HANDLE hFile = CreateFile(getTempPath().a_str(), GENERIC_WRITE, 0, NULL, (overwritte) ? TRUNCATE_EXISTING : OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
      if (hFile == INVALID_HANDLE_VALUE) {
        if (overwritte) return openTemp(false);
        else return false;
      }
      _file = hFile;
      _opened = true;
      _temp_opened = true;
      return true;
    }

    virtual inline bool isOpenTemp() const {
      return _temp_opened;
    }

    virtual inline bool remove() {
      Stamina::LockerCS locker(_locker);
      return DeleteFile(getPath().a_str());
    }

    virtual inline bool removeTemp() {
      Stamina::LockerCS locker(_locker);
      return DeleteFile(getTempPath().a_str());
    }

    virtual inline bool isExists() {
      Stamina::LockerCS locker(_locker);
      DWORD code = getAttrib(getPath());
      return (code != -1) && ((FILE_ATTRIBUTE_DIRECTORY & code) == 0);
    }

    virtual inline bool isTempExists() {
      Stamina::LockerCS locker(_locker);
      DWORD code = getAttrib(getPath() + "\\" + getTempName());
      return (code != -1) && ((FILE_ATTRIBUTE_DIRECTORY & code) == 0);
    }

    virtual inline bool isOpened() {
      Stamina::LockerCS locker(_locker);
      return _opened;
    }

    virtual inline bool close() {
      Stamina::LockerCS locker(_locker);
      if (!isOpened()) return false;
      _opened = false;
      return CloseHandle(_file);
    }

    virtual bool write(DWORD offset, void* buffer, DWORD size) {
      Stamina::LockerCS locker(_locker);
      if ((size == 0) || !isOpened() || !buffer || !setSeek(offset)) return 0;
	    DWORD bytes_written;	
	    bool success = WriteFile(_file, buffer, size, &bytes_written, NULL);
	    if (!success) {
        return 0;	
      }
      return bytes_written;
    }

    virtual bool read(DWORD offset, void* buffer, DWORD size) {
      Stamina::LockerCS locker(_locker);
      if ((size == 0) || !isOpened() || !buffer || !setSeek(offset)) return 0;
      DWORD bytes_read; 
	    bool success = ReadFile(_file, buffer, size, &bytes_read, NULL); 
	    if (success && (!bytes_read)) { // eof
        return 0;
      }
      if(!success) {
        return 0;
      }
      return bytes_read;
    }

    virtual bool setSeek(DWORD offset) const {
	    DWORD dwptr =  SetFilePointer(_file, offset, NULL, FILE_BEGIN);
	    if (dwptr == 0xFFFFFF) return false;
      return true;
    }

    virtual DWORD getSize() {
      Stamina::LockerCS locker(_locker);
      if (!isOpened()) return 0;
      DWORD size = GetFileSize(_file, NULL);
      if (size == INVALID_FILE_SIZE) return 0;
	    return size;
    }

  private:
    String _temp_name;
    HANDLE _file;
    bool _opened;
    bool _temp_opened;
  };
};

#endif /*__FILE_H__*/