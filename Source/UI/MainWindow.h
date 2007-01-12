/**
  *  kTransfer3 UI - Main Window Class
  *
  *  @filesource
  *  @copyright    Copyright (c) 2006-2007 Ursus6
  *  @link         svn://konnekt.info/ktransfer3/ kTransfer3 plugin SVN Repo
  *  @version      $Revision$
  *  @modifiedby   $LastChangedBy$
  *  @lastmodified $Date$
  */

#pragma once

#ifndef __MAIN_WINDOW_H__
#define __MAIN_WINDOW_H__

#include "stdafx.h"
using namespace SmartWin;

namespace kTransfer3 {
  class MainWindow: public iObject, public WidgetFactory<WidgetWindow, MainWindow> {
  public:
    STAMINA_OBJECT_CLASS_VERSION(MainWindow, iObject, Version(0,1,0,0));

  public:
    MainWindow() {
      _kill = false;
    }

    void init() {
      int defx = 500, defy = 300;
      wnd.caption = _T("kTransfer3");
      wnd.location = SmartWin::Rectangle(0, 0, 0, 0);
      createWindow(wnd);
      setVisible(false);
  
      onSized(&MainWindow::_onResized);
      onRaw(&MainWindow::_onSizing, Message(WM_SIZING));
      onRaw(&MainWindow::_onClosing, Message(WM_CLOSE));

      lv = new ListWnd::ListView(0, 0, 0, 0, handle(), 0);
      hListView = lv->getHwnd();
      lv->alwaysShowScrollbars(false, true);

      setBounds(0, 0, defx, defy, true);
    }

    void killWindow() {
      _kill = true;
    }

  private:
    HRESULT _onClosing(LPARAM lPar, WPARAM wPar) {
      if (_kill) return ::DefWindowProc(handle(), WM_CLOSE, wPar, lPar);
      setVisible(false);
      return 1;
    }
    
    void _onResized(const WidgetSizedEventResult& sz) {
      MoveWindow(hListView, 0, 0, sz.newSize.x, sz.newSize.y, true);
	  }

    HRESULT _onSizing(LPARAM lPar, WPARAM wPar) {
      if (!lPar) return 0;
      PRECT rc = (PRECT)lPar;

      if (rc->right - rc->left < min_x) {
        if (rc->left == getPosition().x) {
          rc->right = rc->left + min_x;
        } else {
          rc->left = rc->right - min_x;
        }
      }
      if (rc->bottom - rc->top < min_y) {
        if (rc->top == getPosition().y) {
          rc->bottom = rc->top + min_y;
        } else {
          rc->top = rc->bottom - min_y;
        }
      }
      return 1;
    }

   
  public:
    static const int min_x = 500;
    static const int min_y = 300;

  private:
    WidgetWindow::Seed wnd;
    HWND hListView;
    ListWnd::ListView* lv;

    bool _kill;
  };

  MainWindow* main_wnd = new MainWindow;

  unsigned int MainLoop(void *arg) {
    main_wnd->init();
    SmartWin::Application::instance().run();

    bool corruptMemMemLeak;
    try {
      Application::checkCorruptOrMemleak(corruptMemMemLeak);
    } catch ( xCeption & err ) {
			Application::reportErr( err, corruptMemMemLeak );
		}
    return 0;
  }
};



#endif /*__MAIN_WINDOW_H__*/

