#ifndef __EASYWIN_IEVENT_H__
#define __EASYWIN_IEVENT_H__

#include "easyWinDefines.h"

namespace easyWin {
  class iEvent : public iObject {
  public: 
    iEvent() {}
    virtual ~iEvent() {}

    tMsgCallback onPaint(eMsg *msg); // WM_PAINT
    tMsgCallback onShow(eMsg *msg); // WM_SHOW
    tMsgCallback onCreate(eMsg *msg); // WM_CREATE
    tMsgCallback onDestroy(eMsg *msg); // WM_DESTROY
    tMsgCallback onSize(eMsg *msg); // WM_SIZE
    tMsgCallback onMove(eMsg *msg); // WM_MOVE
    tMsgCallback onKeyDown(eMsg *msg); // WM_KEYDOWN
    tMsgCallback onKeyUp(eMsg *msg); // WM_KEYUP
    tMsgCallback onKeyPress(eMsg *msg); // WM_CHAR
    tMsgCallback onClick(eMsg *msg); // WM_LBUTTONDOWN, WM_MBUTTONDOWN, WM_RBUTTONDOWN
    tMsgCallback onDblClick(eMsg *msg); // WM_LBUTTONDBLCLK, WM_MBUTTONDBLCLK, WM_RBUTTONDBLCLK
    tMsgCallback onMouseMove(eMsg *msg); // WM_MOUSEMOVE
    tMsgCallback onMouseUp(eMsg *msg); // WM_LBUTTONUP,  WM_MBUTTONUP,  WM_RBUTTONUP
    tMsgCallback onMouseLeave(eMsg *msg); // WM_MOUSELEAVE //option
    tMsgCallback onMouseHover(eMsg *msg); // WM_MOUSEHOVER //option

    signal<tEventCallback(HDC hDC, LPPAINTSTRUCT ps)> evtOnPaint; 
    signal<tEventCallback(showOption sho, bool show)> evtOnShow;
    signal<tEventCallback(CREATESTRUCT *cs)> evtOnCreate;
    signal<tEventCallback()> evtOnDestroy; 
    signal<tEventCallback(sizeOption so, int w, int h)> evtOnSize; 
    signal<tEventCallback(int x, int y)> evtOnMove; 
    signal<tEventCallback(int nsKey, int vKey)> evtOnKeyDown;
    signal<tEventCallback(int nsKey, int vKey)> evtOnKeyUp;
    signal<tEventCallback(int nsKey, int vKey)> evtOnKeyPress; 
    signal<tEventCallback(mouseButton mb, int x, int y)> evtOnMouseMove;
    signal<tEventCallback(mouseButton mb, int x, int y)> evtOnMouseLeave;
    signal<tEventCallback(mouseButton mb, mouseButton mbn /*sasiedzi*/, int x, int y)> evtOnMouseUp;
    signal<tEventCallback(mouseButton mb, mouseButton mbn /*sasiedzi*/, int x, int y)> evtOnClick;
    signal<tEventCallback(mouseButton mb, mouseButton mbn /*sasiedzi*/, int x, int y)> evtOnDblClick;  
  };
};

#endif /*__EASYWIN_IEVENT_H__*/