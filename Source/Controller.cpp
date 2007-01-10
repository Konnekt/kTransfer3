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

#include "stdafx.h"
#include "Controller.h"

namespace kTransfer3 {
  Controller::Controller() {
    setStaticValue(IM_PLUG_NET, net);
    setStaticValue(IM_PLUG_TYPE, IMT_CONFIG | IMT_UI);
    setStaticValue(IM_PLUG_SIG, (int)"KT3");
    setStaticValue(IM_PLUG_NAME, (int) "kTransfer3");
    setStaticValue(IM_PLUG_PRIORITY, PLUGP_HIGH);
    setStaticValue(IM_PLUG_DONTFREELIBRARY, 1);

    registerObserver(IM_START, bind(resolve_cast0(&Controller::_iStart), this));
    registerObserver(IM_UI_PREPARE, bind(resolve_cast0(&Controller::_onPrepare), this));
    registerObserver(IM_BEFOREEND, bind(resolve_cast0(&Controller::_onBeforeEnd), this));


    registerObserver(im::transferAdd, bind(resolve_cast0(&Controller::_onTransferAdd), this));
    registerObserver(im::transferDelete, bind(resolve_cast0(&Controller::_onTransferRemove), this));
    registerObserver(im::transferIs, bind(resolve_cast0(&Controller::_onTransferIs), this));
    registerObserver(im::transferGet, bind(resolve_cast0(&Controller::_onTransferGet), this)); 
    registerObserver(im::transferQuery, bind(resolve_cast0(&Controller::_onTransferQuery), this)); 
    registerObserver(im::transferRefresh, bind(resolve_cast0(&Controller::_onTransferRefresh), this)); 
    registerObserver(im::transferCount, bind(resolve_cast0(&Controller::_onTransferCount), this)); 

    registerObserver(im::transferRegisterPlug, bind(resolve_cast0(&Controller::_onTransferRegisterPlug), this)); 
    registerObserver(im::transferDeletePlug, bind(resolve_cast0(&Controller::_onTransferDeletePlug), this)); 

    registerActionObserver(ui::transferButton, bind(resolve_cast0(&Controller::_onPressTransferButton), this));
  }

  tIMCallback Controller::_onPrepare() {
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
    UIActionCfgAdd(ui::cfgGroup, cfg::adAutoAccept, ACTT_COMBO | ACTSCOMBO_LIST | ACTSCOMBO_BYPOS, "Zawsze pytaj" CFGICO "11"
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

    UIActionAdd(ICMessage(IMI_GETPLUGINSGROUP), ui::transferButton, 0, "Transfery", ico::logo);
  }


};

