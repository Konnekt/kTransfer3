/**
  *  kTransfer3 - Controller Class
  *
  *  @filesource
  *  @copyright    Copyright (c) 2006-2007 Ursus6
  *  @link         svn://konnekt.info/ktransfer3/ kTransfer3 plugin SVN Repo
  *  @version      $Revision$
  *  @modifiedby   $LastChangedBy$
  *  @lastmodified $Date$
  */

#pragma once

#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

#include "stdafx.h"
#include "Transfer.h"
#include "Queue.h"

using namespace Stamina;

namespace kTransfer3 {
  class Controller: public PlugController<Controller> {
  public:
    struct sTransferPlug {
      int plug_id;
      int net;

      sTransferPlug(int plug_id, int net): plug_id(plug_id), net(net) {}
    };

    typedef std::vector<sTransferPlug> tTransfersPlugs;

  public:
    friend class PlugController<Controller>;

  public:
    STAMINA_OBJECT_CLASS_VERSION(Controller, PlugController, Version(0,1,0,0));

  protected:
    Controller(); 

  public:
    ~Controller() {
    }

  private:
    tIMCallback _iStart() {
    }

    tIMCallback _onTransferAdd() {
      _queue.insertTransfer((Transfer*)getIM()->p1);
    }

    tIMCallback _onTransferRemove() {
      setReturnCode(_queue.removeTransfer(getIM()->p1));
    }

    tIMCallback _onTransferIs() {
      setReturnCode(_queue.haveTransfer(getIM()->p1));
    }

    tIMCallback _onTransferGet() {
      if (getIM()->p2) {
        setReturnCode((int)_queue.getTransferT(getIM()->p1).get());
      }
      else {
        setReturnCode((int)_queue.getTransfer(getIM()->p1).get());
      }
    }

    tIMCallback _onTransferRefresh() {
      _queue.refreshTransfer(getIM()->p1);
    } 

    tIMCallback _onTransferCount() {
      _queue.countTransfer();
    }

    tIMCallback _onTransferQuery() {
    }

    bool haveTransferPlug(int plug_id, int net) {
      tTransfersPlugs::iterator it = _plugs.begin();
      for (;it != _plugs.end(); it++) {
        if (((*it).plug_id == plug_id) && ((*it).net == net)) return true;
      }
      return false;
    }

    tIMCallback _onTransferRegisterPlug() {
      if (haveTransferPlug(getIM()->sender, getIM()->p1)) return setFailure();
      sTransferPlug tp(getIM()->sender, getIM()->p1);
      _plugs.push_back(tp);
      setSuccess();
    }

    tIMCallback _onTransferDeletePlug() {
      if (!haveTransferPlug(getIM()->sender, getIM()->p1)) return setFailure(); 
      tTransfersPlugs::iterator it = _plugs.begin();
      for (;it != _plugs.end(); it++) {
        if (((*it).plug_id == getIM()->sender) && ((*it).net == getIM()->p1)) {
          _plugs.erase(it);
          setSuccess();
        }
      }
      setFailure();
    }

    tIMCallback _onPressTransferButton() {
      main_wnd->setVisible(true);
    }

    inline bool _isTransferPlugHandling(int net) {
      tTransfersPlugs::iterator it = _plugs.begin();
      for (;it != _plugs.end(); it++) {
        if ((*it).net == net) return true;
      }
      return false;
    }

    inline bool _isTransferPlugHandlingType(Transfer::enType type) {
      tTransfersPlugs::iterator it = _plugs.begin();
      for (;it != _plugs.end(); it++) {
        if (IMessageDirect(im::transferGetType, (*it).plug_id, (*it).net, type) == 1) {
          return true;
        }
      }
      return false;
    }

    inline void _setPluginsInfoText() {
      Stamina::String text = formatedString("Iloœæ podpiêtych handlerów: <b>%d</b><br/>", _plugs.size()) + \
      formatedString("Lista wtyczek obs³uguj¹cych transfery:<br/>");

      tTransfersPlugs::iterator it = _plugs.begin();
      char ver[100]={0};
      for (;it != _plugs.end(); it++) {
        ICMessage(IMC_PLUG_VERSION, ICMessage(IMC_PLUGID_POS, (*it).plug_id, 0), (int) ver);
        text+= formatedString("<b>%s</b> - %s<br/>Obs³uga:", SAFECHAR((char*)IMessageDirect(IM_PLUG_NAME, (*it).plug_id, 0, 0)), ver);
        
        Stamina::String handling;
        if (IMessageDirect(im::transferGetType, (*it).plug_id, (*it).net, Transfer::enType::typeFile) == 1) handling = " plików";
        if (IMessageDirect(im::transferGetType, (*it).plug_id, (*it).net, Transfer::enType::typeFilesFolders) == 1) handling+= " ,katalogów";
        if (IMessageDirect(im::transferGetType, (*it).plug_id, (*it).net, Transfer::enType::typeImage) == 1) handling+= (!handling.size())? " obrazków ":" ,obrazków";
        if (!handling.size()) handling = " nieznane/brak";
        text+= handling + ".<br/>";
      }

      UIActionSetText(ui::cfgGroup, ui::plugsInfo, text.a_str());
    }
    
    tIMCallback _onPrepare();

  private:
    Queue _queue;
    tTransfersPlugs _plugs;
  };
};

#endif /*__CONTROLLER_H__*/