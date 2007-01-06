/**
  *  kTransfer3 UI - Main Window Class
  *
  *  @filesource
  *  @copyright    Copyright (c) 2006-2007 Ursus6
  *  @link         svn://konnekt.info/ktransfer3/ kTransfer3 plugin SVN Repo
  *  @version      $Revision: 27 $
  *  @modifiedby   $LastChangedBy: ursus6 $
  *  @lastmodified $Date: 2007-01-02 08:25:56 +0100 (Wt, 02 sty 2007) $
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
    }

    void init() {
      int defx = 500, defy = 300;
      wnd.caption = _T("kTransfer3");
      wnd.location = SmartWin::Rectangle(0, 0, 0, 0);
      createWindow(wnd);
      //setVisible(false);
      onSized(&MainWindow::_onResized);
      onClosing(&MainWindow::_onClosing);
      onRaw(&MainWindow::_onSizing, Message(WM_SIZING));
      onRaw(&MainWindow::_onDestroy, Message(WM_DESTROY));

      lv = new ListWnd::ListView(0, 0, 0, 0, handle(), 0);
      hListView = lv->getHwnd();
      lv->alwaysShowScrollbars(false, true);

      MoveWindow(handle(), 0, 0, defx, defy, true);

      ListWnd::oItem coll = lv->insertEntry(new RecvCollectionEntry(new Transfer(kTransfer3::Transfer::enType::typeFile)));
      lv->insertEntry(new RecvCollectionEntry(new Transfer(kTransfer3::Transfer::enType::typeFile)));
      lv->insertEntry(new RecvCollectionEntry(new Transfer(kTransfer3::Transfer::enType::typeFile)));
      lv->insertEntry(new RecvCollectionEntry(new Transfer(kTransfer3::Transfer::enType::typeFile)));
      lv->insertEntry(new RecvCollectionEntry(new Transfer(kTransfer3::Transfer::enType::typeFile)));
      lv->insertEntry(new RecvCollectionEntry(new Transfer(kTransfer3::Transfer::enType::typeFile)));
      lv->insertEntry(new RecvCollectionEntry(new Transfer(kTransfer3::Transfer::enType::typeFile)));
      lv->insertEntry(new RecvCollectionEntry(new Transfer(kTransfer3::Transfer::enType::typeFile)));
      coll->setFlag(ListWnd::flagExpanded, true);
    }

  private:
    bool _onClosing() {
      setVisible(false);
      return false;
    }
    
    void _onResized(const WidgetSizedEventResult& sz) {
      MoveWindow(hListView, 0, 0, sz.newSize.x, sz.newSize.y, true);
	  }

    HRESULT _onSizing(LPARAM lPar, WPARAM wPar) {
      if (!lPar) return 0;
      PRECT rc = (PRECT)lPar;
      if (rc->right - rc->left < min_x) rc->right = rc->left + min_x;
      if (rc->bottom - rc->top < min_y) rc->bottom = rc->top + min_y;
      return 1;
    }

    HRESULT _onDestroy(LPARAM lPar, WPARAM wPar) {
      delete lv;
      return 1;
    }
    
  public:
    static const int min_x = 500;
    static const int min_y = 300;

  private:
    WidgetWindow::Seed wnd;
    HWND hListView;
    ListWnd::ListView *lv;
  };

  MainWindow* main_wnd;

  unsigned int MainLoop(void *arg) {
    main_wnd = new MainWindow;
    main_wnd->init();
    return SmartWin::Application::instance().run();
  }
};



#endif /*__MAIN_WINDOW_H__*/

