#ifndef __EASYWIN_DEFINES_H__
#define __EASYWIN_DEFINES_H__

#define BOOST_MEM_FN_ENABLE_STDCALL
#include <windows.h>
#include <assert.h>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/signal.hpp>

using namespace boost;

namespace easyWin {

  typedef void tMsgCallback;
  typedef void tEventCallback;

  struct eMsg {
    HWND hWnd;
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    eMsg() {};
    eMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam): hWnd(hWnd), msg(msg), wParam(wParam), lParam(lParam) {};
  };

  typedef function<tMsgCallback(eMsg *msg)> fOnMessage;
  typedef signal<tMsgCallback(eMsg *msg)> sigOnMessage;

  enum sizeOption{soMaxHide, soMaximized, soMaxShow, soMinimized, soRestored};
  enum mouseButton{mbLeft = MK_LBUTTON, mbMiddle = MK_MBUTTON, mbRight = MK_RBUTTON, mbControl = MK_CONTROL, mbShift = MK_SHIFT};
  enum showOption{shOtherUnZoom, shOtherZoom, shParentClosing, shParentOpening};



};
#endif /*__EASYWIN_DEFINES_H__*/