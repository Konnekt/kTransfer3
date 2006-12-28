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
    Controller() {
      setStaticValue(IM_PLUG_NET, net);
      setStaticValue(IM_PLUG_TYPE, IMT_CONFIG | IMT_UI);
      setStaticValue(IM_PLUG_SIG, (int)"KT3");
      setStaticValue(IM_PLUG_NAME, (int) "kTransfer3");
      setStaticValue(IM_PLUG_PRIORITY, PLUGP_HIGH);

      registerObserver(imi::transferAdd, bind(resolve_cast0(&Controller::_onTransferAdd), this));
      registerObserver(imi::transferDelete, bind(resolve_cast0(&Controller::_onTransferRemove), this));
      registerObserver(imi::transferIs, bind(resolve_cast0(&Controller::_onTransferIs), this));
      registerObserver(imi::transferGet, bind(resolve_cast0(&Controller::_onTransferGet), this)); 
      registerObserver(imi::transferQuery, bind(resolve_cast0(&Controller::_onTransferQuery), this)); 
      registerObserver(imi::transferRefresh, bind(resolve_cast0(&Controller::_onTransferRefresh), this)); 
      registerObserver(imi::transferCount, bind(resolve_cast0(&Controller::_onTransferCount), this)); 

      registerObserver(im::transferRegisterPlug, bind(resolve_cast0(&Controller::_onTransferRegisterPlug), this)); 
      registerObserver(im::transferDeletePlug, bind(resolve_cast0(&Controller::_onTransferDeletePlug), this)); 

      registerObserver(IM_UI_PREPARE, bind(resolve_cast0(&Controller::_onPrepare), this));
    }

  public:
    ~Controller();

  private:
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
      for (;it != _plugs.end(); it++) {
        text+= formatedString("%s<br/>",SAFECHAR((char*)IMessageDirect(IM_PLUG_NAME, (*it)->plug_id, 0, 0));
      }
    }

    tIMCallback _onPrepare() {
      UIGroupAdd(IMIG_CFG_PLUGS, ui::cfgGroup, 0, "kTransfer3", 13);
      
      UIActionCfgAddPluginInfoBox2(ui::cfgGroup, 
      "Wtyczka <b>kTransfer3</b> udostêpnia UI do kontroli i zarz¹dzania transferami.",
      "<span class='note'>Skompilowano: <b>" __DATE__ "</b> [<b>" __TIME__ "</b>]</span><br/>"
      "Copyright © 2006-2007 <b>Ursus6</b><br/>", "0");

      UIActionCfgAdd(ui::cfgGroup, 0, ACTT_GROUP, "Informacje");


      UIActionCfgAdd(ui::cfgGroup, 0 , ACTT_HTMLINFO | ACTSC_FULLWIDTH, text.a_str(), 0, 0, 0, 300, 300);
      UIActionCfgAdd(ui::cfgGroup, 0, ACTT_GROUPEND);

      UIActionCfgAdd(ui::cfgGroup, 0, ACTT_GROUP, "Ustawienia");
      UIActionCfgAdd(ui::cfgGroup, 0, ACTT_GROUPEND);
    }
  private:
    Queue _queue;
    tTransfersPlugs _plugs;

  };
};

#endif /*__CONTROLLER_H__*/