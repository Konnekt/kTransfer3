#include"stdafx.h"
#include"easyWinDefines.h"
#include"iEvent.h"
#include <commctrl.h>

using namespace easyWin;

tMsgCallback iEvent::onClick(eMsg *msg) {
  switch (msg->msg) {
    case WM_LBUTTONDOWN:
      evtOnClick(mbLeft, (mouseButton)msg->wParam, LOWORD(msg->lParam), HIWORD(msg->lParam));
    break;
    case WM_RBUTTONDOWN:
      evtOnClick(mbRight, (mouseButton)msg->wParam, LOWORD(msg->lParam), HIWORD(msg->lParam));
    break;
    case WM_MBUTTONDOWN:
      evtOnClick(mbMiddle, (mouseButton)msg->wParam, LOWORD(msg->lParam), HIWORD(msg->lParam));
    break;
  }
}

tMsgCallback iEvent::onDblClick(eMsg *msg) {
  switch (msg->msg) {
    case WM_LBUTTONDBLCLK:
      evtOnDblClick(mbLeft, (mouseButton)msg->wParam, LOWORD(msg->lParam), HIWORD(msg->lParam));
    break;
    case WM_MBUTTONDBLCLK:
      evtOnDblClick(mbRight, (mouseButton)msg->wParam, LOWORD(msg->lParam), HIWORD(msg->lParam));
    break;
    case WM_RBUTTONDBLCLK:
      evtOnDblClick(mbMiddle, (mouseButton)msg->wParam, LOWORD(msg->lParam), HIWORD(msg->lParam));
    break;
  }
}

tMsgCallback iEvent::onMouseMove(eMsg *msg) {
  evtOnMouseMove((mouseButton)msg->wParam, LOWORD(msg->lParam), HIWORD(msg->lParam));
}

tMsgCallback iEvent::onMouseUp(eMsg *msg) {
  switch (msg->msg) {
    case WM_LBUTTONUP:
      evtOnMouseUp(mbLeft, (mouseButton)msg->wParam, LOWORD(msg->lParam), HIWORD(msg->lParam));
    break;
    case WM_RBUTTONUP:
      evtOnMouseUp(mbRight, (mouseButton)msg->wParam, LOWORD(msg->lParam), HIWORD(msg->lParam));
    break;
    case WM_MBUTTONUP:
      evtOnMouseUp(mbMiddle, (mouseButton)msg->wParam, LOWORD(msg->lParam), HIWORD(msg->lParam));
    break;
  }
}

tMsgCallback iEvent::onSize(eMsg *msg) {

  sizeOption option = (sizeOption)0;

  switch (msg->wParam) {
    case SIZE_MAXHIDE:
      option = soMaxHide;
    break;
    case SIZE_MAXIMIZED:
      option = soMaximized;
    break;
    case SIZE_MAXSHOW:
      option = soMaxShow;
    break;
    case SIZE_MINIMIZED:
      option = soMinimized;
    break;
    case SIZE_RESTORED:
      option = soRestored;
    break;
  }
  evtOnSize(option, LOWORD(msg->lParam), HIWORD(msg->lParam));
}

tMsgCallback iEvent::onMove(eMsg *msg) {
  evtOnMove(LOWORD(msg->lParam), HIWORD(msg->lParam));
}

tMsgCallback iEvent::onCreate(eMsg *msg) {
  evtOnCreate((CREATESTRUCT*)msg->lParam);
}

tMsgCallback iEvent::onDestroy(eMsg *msg) {
  evtOnDestroy();
}


tMsgCallback iEvent::onShow(eMsg *msg) {
  showOption option = (showOption)0;
  switch (msg->lParam) {
    case SW_OTHERUNZOOM:
      option = shOtherUnZoom;
    break;
    case SW_OTHERZOOM:
      option = shOtherZoom;
    break;
    case SW_PARENTCLOSING:
      option = shParentClosing;
    break;
    case SW_PARENTOPENING:
      option = shParentOpening;
    break;
  }
  evtOnShow(option, (bool)msg->wParam);
}

tMsgCallback iEvent::onPaint(eMsg *msg) {
  PAINTSTRUCT ps;
  HDC hDC = BeginPaint(msg->hWnd, &ps);
  evtOnPaint(hDC, &ps);
  EndPaint(msg->hWnd, &ps);
}

tMsgCallback iEvent::onKeyDown(eMsg *msg) {
  evtOnKeyDown(msg->wParam, msg->lParam);
}

tMsgCallback iEvent::onKeyUp(eMsg *msg) {
  evtOnKeyUp(msg->wParam, msg->lParam);
}

tMsgCallback iEvent::onKeyPress(eMsg *msg) {
  evtOnKeyPress(msg->wParam, msg->lParam);
}

