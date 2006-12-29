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

      registerObserver(im::transferAdd, bind(resolve_cast0(&Controller::_onTransferAdd), this));
      registerObserver(im::transferDelete, bind(resolve_cast0(&Controller::_onTransferRemove), this));
      registerObserver(im::transferIs, bind(resolve_cast0(&Controller::_onTransferIs), this));
      registerObserver(im::transferGet, bind(resolve_cast0(&Controller::_onTransferGet), this)); 
      registerObserver(im::transferQuery, bind(resolve_cast0(&Controller::_onTransferQuery), this)); 
      registerObserver(im::transferRefresh, bind(resolve_cast0(&Controller::_onTransferRefresh), this)); 
      registerObserver(im::transferCount, bind(resolve_cast0(&Controller::_onTransferCount), this)); 

      registerObserver(im::transferRegisterPlug, bind(resolve_cast0(&Controller::_onTransferRegisterPlug), this)); 
      registerObserver(im::transferDeletePlug, bind(resolve_cast0(&Controller::_onTransferDeletePlug), this)); 

      registerObserver(IM_UI_PREPARE, bind(resolve_cast0(&Controller::_onPrepare), this));

      /* test
      Transfer *tr = new Transfer(Transfer::enType::typeFilesFolders);
      tr->addItem(new Directory("dir1",(Directory*)tr));
      ((Directory*)tr->getItemT(0))->addFile("file.xxx");
      tr->setSavePath("c:\\root");
      Stamina::String txt = ((Directory*)tr->getItemT(0))->getFileT(0)->getPath();
      */
      
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

    tIMCallback _onPrepare() {
      IconRegister(IML_16, ico::logo, Ctrl->hDll(), IDI_LOGO);

      UIGroupAdd(IMIG_CFG_PLUGS, ui::cfgGroup, 0, "kTransfer3", ico::logo);
     
      UIActionCfgAddPluginInfoBox2(ui::cfgGroup, 
      "Wtyczka <b>kTransfer3</b> udostêpnia UI do kontroli i zarz¹dzania transferami.",
      "<span class='note'>Skompilowano: <b>" __DATE__ "</b> [<b>" __TIME__ "</b>]</span><br/>"
      "Copyright © 2006-2007 <b>Ursus6</b><br/>", formatedString("reg://IML16/%d.ico", ico::logo).a_str());

      UIActionCfgAdd(ui::cfgGroup, 0, ACTT_GROUP, "Informacje");
      UIActionCfgAdd(ui::cfgGroup, ui::plugsInfo, ACTT_HTMLINFO | ACTSC_FULLWIDTH, "", 0, 0, 0, 300, 300);
      UIActionCfgAdd(ui::cfgGroup, 0, ACTT_GROUPEND);
      sTransferPlug plug(Ctrl->ID(), 130);
      _plugs.push_back(plug);
      _setPluginsInfoText();

      UIActionCfgAdd(ui::cfgGroup, 0, ACTT_GROUP, "Przekierowanie");
      UIActionCfgAdd(ui::cfgGroup, cfg::fwNAT, ACTT_CHECK, "Jestem za NAT'em(IP prywatne)", cfg::fwNAT);
      UIActionCfgAdd(ui::cfgGroup, cfg::fwHaveFowarding, ACTT_CHECK, "Posiadam przekierowany port(fowardowanie)", cfg::fwHaveFowarding);
      UIActionCfgAdd(ui::cfgGroup, 0, ACTT_COMMENT, "IP:PORT [tylko przy w³¹czonym fowardowaniu]");
      UIActionCfgAdd(ui::cfgGroup, cfg::fwIP, ACTT_EDIT | ACTSC_INLINE, "" , cfg::fwIP, 0, 0, 80);
      UIActionCfgAdd(ui::cfgGroup, 0, ACTT_COMMENT | ACTSC_INLINE, " : ", 0, 0, 0, 10);
      UIActionCfgAdd(ui::cfgGroup, cfg::fwPort, ACTT_EDIT | ACTSC_INT, 0, cfg::fwPort, 0, 0, 40);
      UIActionCfgAdd(ui::cfgGroup, 0, ACTT_GROUPEND);

      UIActionCfgAdd(ui::cfgGroup, 0, ACTT_GROUP, "Opcje"); 
      UIActionCfgAdd(ui::cfgGroup, cfg::opSendToInvisible, ACTT_CHECK, "Pozwalaj na przesy³anie do osób niewidocznych", cfg::opSendToInvisible);
      UIActionCfgAdd(ui::cfgGroup, cfg::opInformCommingTransfer, ACTT_CHECK,"Informuj na liœcie kontaktów o nadejœciu transferu", cfg::opInformCommingTransfer);
      UIActionCfgAdd(ui::cfgGroup, cfg::opTransferInMsgWindow, ACTT_CHECK,"Pokazuj w oknie rozmowy informacje o transferze", cfg::opTransferInMsgWindow);
      UIActionCfgAdd(ui::cfgGroup, cfg::opShowOnUserAction, ACTT_CHECK, "Pokazuj okno przy akcji zwi¹zanej z transferami", cfg::opShowOnUserAction);
      UIActionCfgAdd(ui::cfgGroup, cfg::opUsekNotify, ACTT_CHECK, "Informuj o zdarzeniach transferu poprzez kNotify", cfg::opUsekNotify);
      UIActionCfgAdd(ui::cfgGroup, cfg::opSaveToHistory, ACTT_CHECK, "Zapisuj do historii", cfg::opSaveToHistory);
      UIActionCfgAdd(ui::cfgGroup, cfg::opAutoResume, ACTT_CHECK, "Pozwalaj na kontynuacjê (doci¹ganie) nieodebranych plików", cfg::opAutoResume);
      UIActionCfgAdd(ui::cfgGroup, 0, ACTT_GROUPEND);

      UIActionCfgAdd(ui::cfgGroup,0, ACTT_GROUP, "Pobieranie automatyczne");
      UIActionCfgAdd(ui::cfgGroup, cfg::adAutoAccept, ACTT_COMBO | ACTSCOMBO_LIST | ACTSCOMBO_BYPOS |ACTSC_INLINE, "Zawsze pytaj" CFGICO "11"
      "\n" "Akceptuj wszystko" CFGICO "20" "\n" "Odrzucaj wszystko" CFGICO "46" "\n", cfg::adAutoAccept, 0, 0, 150);
      UIActionCfgAdd(ui::cfgGroup, cfg::adUseDefaultDir, ACTT_CHECK, "U¿ywaj domyœlnej œcie¿ki", cfg::adUseDefaultDir);
      UIActionCfgAdd(ui::cfgGroup, 0, ACTT_COMMENT, "Katalog sk³adowania plików (pe³na œcie¿ka):" CFGTIP "%konnektProfile% - katalog profilu");
      UIActionCfgAdd(ui::cfgGroup, cfg::adDefaultDir, ACTT_DIR, 0, cfg::adDefaultDir);
      UIActionCfgAdd(ui::cfgGroup, cfg::adMinDriveSpace, ACTT_EDIT | ACTSC_INT | ACTSC_INLINE, 0, cfg::adMinDriveSpace, 0, 0, 50);
      UIActionCfgAdd(ui::cfgGroup, 0, ACTT_COMMENT, " - Limit wolnego miejsca na dysku w MB");
      UIActionCfgAdd(ui::cfgGroup, 0, ACTT_GROUPEND);

      UIGroupAdd(IMIG_NFO, ui::userCfgGroup, ACTR_INIT, "kTransfer3", ico::logo); 
      UIActionCfgAdd(ui::userCfgGroup, 0, ACTT_GROUP, "Pobieranie automatyczne"); 
      UIActionCfgAdd(ui::userCfgGroup, cfg::adUserAutoAccept, ACTT_COMBO | ACTSCOMBO_LIST | ACTSCOMBO_BYPOS, "Domyœlnie" CFGICO "27" "\n" "Zawsze pytaj" CFGICO "11"
      "\n" "Akceptuj wszystko" CFGICO "20" "\n" "Odrzucaj wszystko" CFGICO "46" "\n", cfg::adAutoAccept, 0, 0, 150);
      UIActionCfgAdd(ui::userCfgGroup, 0, ACTT_COMMENT, "Katalog sk³adowania plików (pe³na scie¿ka):");
      UIActionCfgAdd(ui::userCfgGroup, cfg::adUserDefaultDir, ACTT_DIR, 0, cfg::adUserDefaultDir);
      UIActionCfgAdd(ui::userCfgGroup, 0, ACTT_GROUPEND);
    }

  private:
    Queue _queue;
    tTransfersPlugs _plugs;
  };
};

#endif /*__CONTROLLER_H__*/