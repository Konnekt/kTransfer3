#ifndef __EASYWIN_FORM_H__
#define __EASYWIN_FORM_H__

#include "iEvent.h"
#include "Window.h"
#include "easyWinDefines.h"

namespace easyWin {
  class Form : public Window {
  public:
    Form() {
      prepareClass("easyWin::Form");
    }

    void create(const char *name, int style, int exStyle, int x, int y, int w, int h, HWND parent = NULL);
  };
};

#endif /*__WINDOW_H__*/