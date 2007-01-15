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
    setStaticValue(IM_PLUG_SIG, (int) "KT3");
    setStaticValue(IM_PLUG_NAME, (int) "kTransfer3");
    setStaticValue(IM_PLUG_PRIORITY, PLUGP_HIGH);
    //setStaticValue(IM_PLUG_DONTFREELIBRARY, 1);

    registerObserver(IM_START, bind(resolve_cast0(&Controller::_iStart), this));
    registerObserver(IM_UI_PREPARE, bind(resolve_cast0(&Controller::_onPrepare), this));
    registerObserver(IM_BEFOREEND, bind(resolve_cast0(&Controller::_onBeforeEnd), this));

    registerObserver(im::add, bind(resolve_cast0(&Controller::_onTransferAdd), this));
    registerObserver(im::remove, bind(resolve_cast0(&Controller::_onTransferRemove), this));
    registerObserver(im::has, bind(resolve_cast0(&Controller::_onTransferIs), this));
    registerObserver(im::get, bind(resolve_cast0(&Controller::_onTransferGet), this)); 
    registerObserver(im::query, bind(resolve_cast0(&Controller::_onTransferQuery), this)); 
    registerObserver(im::refresh, bind(resolve_cast0(&Controller::_onTransferRefresh), this)); 
    registerObserver(im::count, bind(resolve_cast0(&Controller::_onTransferCount), this)); 

    registerObserver(im::registerPlug, bind(resolve_cast0(&Controller::_onTransferRegisterPlug), this)); 
    registerObserver(im::unregisterPlug, bind(resolve_cast0(&Controller::_onTransferDeletePlug), this)); 

    registerActionObserver(ui::transferButton, bind(resolve_cast0(&Controller::_onPressTransferButton), this));
  }

  tIMCallback Controller::_onPrepare() {
    IconRegister(IML_16, ico::logo, Ctrl->hDll(), IDI_LOGO);

    UIGroupAdd(IMIG_CFG_PLUGS, ui::cfgGroup, 0, "kTransfer3", ico::logo);
   
    UIActionCfgAddPluginInfoBox2(ui::cfgGroup, 
      "Wtyczka <b>kTransfer3</b> udost�pnia UI do kontroli i zarz�dzania transferami.",
      "<span class='note'>Skompilowano: <b>" __DATE__ "</b> [<b>" __TIME__ "</b>]</span><br/>"
      "Copyright � 2006-2007 <b>Ursus6</b><br/>", formatedString("reg://IML16/%d.ico", ico::logo).a_str());

    UIActionCfgAdd(ui::cfgGroup, 0, ACTT_GROUP, "Informacje");
    UIActionCfgAdd(ui::cfgGroup, ui::plugsInfo, ACTT_HTMLINFO | ACTSC_FULLWIDTH, "", 0, 0, 0, 300, 300);
    UIActionCfgAdd(ui::cfgGroup, 0, ACTT_GROUPEND);

    sTransferPlug plug(Ctrl->ID(), net);
    _plugs.push_back(plug);
    _setPluginsInfoText();

    UIActionCfgAdd(ui::cfgGroup, 0, ACTT_GROUP, "Przekierowanie");
    UIActionCfgAdd(ui::cfgGroup, cfg::fwNAT, ACTT_CHECK, "Jestem za NAT'em(IP prywatne)", cfg::fwNAT);
    UIActionCfgAdd(ui::cfgGroup, cfg::fwHaveFowarding, ACTT_CHECK, "Posiadam przekierowany port(fowardowanie)", cfg::fwHaveFowarding);
    UIActionCfgAdd(ui::cfgGroup, 0, ACTT_COMMENT, "IP:PORT [tylko przy w��czonym fowardowaniu]");
    UIActionCfgAdd(ui::cfgGroup, cfg::fwIP, ACTT_EDIT | ACTSC_INLINE, "" , cfg::fwIP, 0, 0, 80);
    UIActionCfgAdd(ui::cfgGroup, 0, ACTT_COMMENT | ACTSC_INLINE, " : ", 0, 0, 0, 10);
    UIActionCfgAdd(ui::cfgGroup, cfg::fwPort, ACTT_EDIT | ACTSC_INT, 0, cfg::fwPort, 0, 0, 40);
    UIActionCfgAdd(ui::cfgGroup, 0, ACTT_GROUPEND);

    UIActionCfgAdd(ui::cfgGroup, 0, ACTT_GROUP, "Opcje"); 
    UIActionCfgAdd(ui::cfgGroup, cfg::opSendToInvisible, ACTT_CHECK, "Pozwalaj na przesy�anie do os�b niewidocznych", cfg::opSendToInvisible);
    UIActionCfgAdd(ui::cfgGroup, cfg::opInformCommingTransfer, ACTT_CHECK,"Informuj na li�cie kontakt�w o nadej�ciu transferu", cfg::opInformCommingTransfer);
    UIActionCfgAdd(ui::cfgGroup, cfg::opTransferInMsgWindow, ACTT_CHECK,"Pokazuj w oknie rozmowy informacje o transferze", cfg::opTransferInMsgWindow);
    UIActionCfgAdd(ui::cfgGroup, cfg::opShowOnUserAction, ACTT_CHECK, "Pokazuj okno przy akcji zwi�zanej z transferami", cfg::opShowOnUserAction);
    UIActionCfgAdd(ui::cfgGroup, cfg::opUsekNotify, ACTT_CHECK, "Informuj o zdarzeniach transferu poprzez kNotify", cfg::opUsekNotify);
    UIActionCfgAdd(ui::cfgGroup, cfg::opSaveToHistory, ACTT_CHECK, "Zapisuj do historii", cfg::opSaveToHistory);
    UIActionCfgAdd(ui::cfgGroup, cfg::opAutoResume, ACTT_CHECK, "Pozwalaj na kontynuacj� (doci�ganie) nieodebranych plik�w", cfg::opAutoResume);
    UIActionCfgAdd(ui::cfgGroup, 0, ACTT_GROUPEND);

    UIActionCfgAdd(ui::cfgGroup,0, ACTT_GROUP, "Pobieranie automatyczne");
    UIActionCfgAdd(ui::cfgGroup, cfg::adAutoAccept, ACTT_COMBO | ACTSCOMBO_LIST | ACTSCOMBO_BYPOS, "Zawsze pytaj" CFGICO "11"
    "\n" "Akceptuj wszystko" CFGICO "20" "\n" "Odrzucaj wszystko" CFGICO "46" "\n", cfg::adAutoAccept, 0, 0, 150);
    UIActionCfgAdd(ui::cfgGroup, cfg::adUseDefaultDir, ACTT_CHECK, "U�ywaj domy�lnej �cie�ki", cfg::adUseDefaultDir);
    UIActionCfgAdd(ui::cfgGroup, 0, ACTT_COMMENT, "Katalog sk�adowania plik�w (pe�na �cie�ka):" CFGTIP "%konnektProfile% - katalog profilu");
    UIActionCfgAdd(ui::cfgGroup, cfg::adDefaultDir, ACTT_DIR, 0, cfg::adDefaultDir);
    UIActionCfgAdd(ui::cfgGroup, cfg::adMinDriveSpace, ACTT_EDIT | ACTSC_INT | ACTSC_INLINE, 0, cfg::adMinDriveSpace, 0, 0, 50);
    UIActionCfgAdd(ui::cfgGroup, 0, ACTT_COMMENT, " - Limit wolnego miejsca na dysku w MB");
    UIActionCfgAdd(ui::cfgGroup, 0, ACTT_GROUPEND);

    UIGroupAdd(IMIG_NFO, ui::cntCfgGroup, ACTR_INIT, "kTransfer3", ico::logo); 
    UIActionCfgAdd(ui::cntCfgGroup, 0, ACTT_GROUP, "Pobieranie automatyczne"); 
    UIActionCfgAdd(ui::cntCfgGroup, cfg::adUserAutoAccept, ACTT_COMBO | ACTSCOMBO_LIST | ACTSCOMBO_BYPOS, "Domy�lnie" CFGICO "27" "\n" "Zawsze pytaj" CFGICO "11"
    "\n" "Akceptuj wszystko" CFGICO "20" "\n" "Odrzucaj wszystko" CFGICO "46" "\n", cfg::adAutoAccept, 0, 0, 150);
    UIActionCfgAdd(ui::cntCfgGroup, 0, ACTT_COMMENT, "Katalog sk�adowania plik�w (pe�na scie�ka):");
    UIActionCfgAdd(ui::cntCfgGroup, cfg::adUserDefaultDir, ACTT_DIR, 0, cfg::adUserDefaultDir);
    UIActionCfgAdd(ui::cntCfgGroup, 0, ACTT_GROUPEND);

    UIActionAdd(Ctrl->ICMessage(IMI_GETPLUGINSGROUP), ui::transferButton, 0, "Transfery", ico::logo);
  }
};