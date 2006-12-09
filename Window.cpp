#include "stdafx.h"
#include "Window.h"
#include <windows.h>

using namespace easyWin;

Window::tMsgProcs Window::_msgProcs;
HINSTANCE easyWin::Window::instance = 0;

__w64 long Window::_sMsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
  if ((msg == WM_NCCREATE) && !_isMsgProc(hWnd)) {
    CREATESTRUCT * cs = (CREATESTRUCT*)lParam;
    Window* window = (Window*)cs->lpCreateParams;
    window->_msgProcs[hWnd] = new sig;
    _msgProcs[hWnd]->signal.connect(boost::bind(&Window::_msgProc, window, _1, _2, _3, _4));
  }
  if (_isMsgProc(hWnd)) {
    return _msgProcs[hWnd]->signal(hWnd, msg, wParam, lParam);
  }
  return DefWindowProc(hWnd, msg, wParam, lParam);
}

__w64 long Window::_msgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
  LPARAM procReturn = process(&eMsg(hWnd, msg, wParam, lParam));
  if (isReturnCodeSet()) {
    return procReturn;
  } else {
    return DefWindowProc(hWnd, msg, wParam, lParam);
  }
}