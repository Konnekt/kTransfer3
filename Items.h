#ifndef __ITEMS_H__
#define __ITEMS_H__

#include "stdafx.h"
#include "Directory.h"
#include "File.h"
#include "Image.h"

class Items: public Directory {
public:
  inline UINT addImage(const Stamina::StringRef &name) {
    if (getFileID(name)) return 0;
    Image* image = new Image();
    image->setName(name);
    _images.push_back(image);
    return image->getID();
  }
  inline bool removeImage(UINT id) {
    tImages::iterator it = _images.begin();
    for (;it != this->_images.end(); it++) {
      if ((*it)->getID() == id ) {
        delete (*it);
        _images.erase(it); 
        return true;
      }
    }
    return false;
  }
  inline Image* getImage(UINT id) {
    tImages::iterator it = _images.begin();
    for (;it != this->_images.end(); it++) {
      if ((*it)->getID() == id ) {
        return (*it);
      }
    }
    return NULL;
  }
  inline Image* getImage(const Stamina::StringRef &name) {
    tImages::iterator it = _images.begin();
    for (;it != this->_images.end(); it++) {
      if ((*it)->getName() == name ) {
        return (*it);
      }
    }
    return NULL;
  }
  inline UINT getImageID(const Stamina::StringRef &name) {
    tImages::iterator it = _images.begin();
    for (;it != _images.end(); it++) {
      if ((*it)->getName() == name ) {
        return (*it)->getID();
      }
    }
    return 0;
  }
  inline void setImagesState(enState state) {
    tImages::iterator it = _images.begin();
    for (;it != _images.end(); it++) {
      (*it)->setState(state);
    }
  }
  inline void setState(enState state) {
    _state = state;
    setImagesState(state);
    setFilesState(state);
    setDirectoriesState(state);
  }

protected:
  tImages _images;

};

#endif /*__ITEMS_H__*/