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
    File(const Stamina::StringRef &name = "", Directory* parent = NULL): Item(name, (Item*)parent) {
      _type = enType::typeFile;
      _opened = false;
      _temp_name = "temp_" + name; // domyslna wartosc
    }

    virtual inline Stamina::String getTempName() {
      return _temp_name;
    }

    virtual bool openFile(bool overwritte) {
      if (isOpened()) return false;
      HANDLE hFile = CreateFile(getPath().a_str(), GENERIC_WRITE, 0, NULL, (overwritte) ? TRUNCATE_EXISTING : OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
      if (hFile == INVALID_HANDLE_VALUE) {
        if (overwritte) return openFile(false);
        else return false;
      }
      _file = hFile;
      _opened = true;      
    }

    virtual Stamina::String getTempPath() {
      Stamina::String path = getPath();
      path.erase(path.findLast("\\"));
      path+= "\\" + getTempName();
      return path;
    }

    virtual bool openTempFile(bool overwritte) {
      if (isOpened()) return false;

      HANDLE hFile = CreateFile(getTempPath().a_str(), GENERIC_WRITE, 0, NULL, (overwritte) ? TRUNCATE_EXISTING : OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
      if (hFile == INVALID_HANDLE_VALUE) {
        if (overwritte) return openFile(false);
        else return false;
      }
      _file = hFile;
      _opened = true;       
    }

    virtual inline DWORD getAttrib(const Stamina::String &path) {
      return GetFileAttributes(path.a_str());
    }

    virtual inline bool isExists() {
      Stamina::String path = getPath() + "\\" + getName();
      DWORD code = getAttrib(path);
      return (code != -1) && ((FILE_ATTRIBUTE_DIRECTORY & code) == 0);
    }

    virtual inline bool isTempExists() {
      Stamina::String path = getPath() + "\\" + getTempName();
      DWORD code = getAttrib(path);
      return (code != -1) && ((FILE_ATTRIBUTE_DIRECTORY & code) == 0);
    }

    virtual inline bool isOpened() {
      return _opened;
    }

    virtual inline bool close() {
      if (!isOpened()) return false;
      _opened = false;
      return CloseHandle(_file);
    }

    virtual bool write(DWORD offset, void* buffer, DWORD size) {
      if ((size == 0) || !isOpened() || !buffer || !setSeek(offset)) return 0;
	    DWORD bytes_written;	
	    bool success = WriteFile(_file, buffer, size, &bytes_written, NULL);
	    if (!success) {
        return 0;	
      }
      return bytes_written;
    }

    virtual bool read(DWORD offset, void* buffer, DWORD size) {
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

    virtual bool setSeek(DWORD offset) {
	    DWORD dwptr =  SetFilePointer(_file, offset, NULL, FILE_BEGIN);
	    if (dwptr == 0xFFFFFF) return 0;
    }

  private:
    Stamina::String _temp_name;
    HANDLE _file;
    bool _opened;
  };
};

#endif /*__FILE_H__*/