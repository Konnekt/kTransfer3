/**
  *  kTransfer3 - Definitions
  *
  *  @filesource
  *  @copyright    Copyright (c) 2006-2007 Ursus6
  *  @link         svn://konnekt.info/ktransfer3/ kTransfer3 plugin SVN Repo
  *  @version      $Revision$
  *  @modifiedby   $LastChangedBy$
  *  @lastmodified $Date$
  */

#pragma once

#ifndef __KTRANSFER3_H__
#define __KTRANSFER3_H__

namespace kTransfer3 {
  const UINT net = 130;
  const UINT net1k = net * 1000;

  namespace cfg {
    const unsigned int cfg = net1k + 500;

    // fowardowanie 
    const unsigned int fwNAT = cfg + 1;
    const unsigned int fwHaveFowarding = cfg + 2;
    const unsigned int fwIP = cfg + 3;
    const unsigned int fwPort = cfg + 4;

    // opcje
    const unsigned int opSendToInvisible = cfg + 10;
    const unsigned int opInformCommingTransfer = cfg + 11;
    const unsigned int opTransferInMsgWindow = cfg + 12;
    const unsigned int opShowOnUserAction = cfg + 13;
    const unsigned int opUsekNotify = cfg + 14;
    const unsigned int opSaveToHistory = cfg + 15;
    const unsigned int opAutoResume = cfg + 16;

    // autopobieranie
    const unsigned int adAutoAccept = cfg + 20;
    const unsigned int adUseDefaultDir = cfg + 21;
    const unsigned int adDefaultDir = cfg + 22;
    const unsigned int adMinDriveSpace = cfg + 23;

    const unsigned int adUserAutoAccept = cfg + 25;
    const unsigned int adUserDefaultDir = cfg + 26;
  };

  namespace ico {
    const unsigned int ico = net1k + 100;

    const unsigned int logo = ico + 1;
  };

  namespace ui {
    const unsigned int ui = net1k;

    const unsigned int cfgGroup = ui + 1;
    const unsigned int cntCfgGroup = ui + 2;

    const unsigned int plugsInfo = ui + 3;
    const unsigned int transferButton = ui + 4;

  };

  namespace act {
  };

  namespace im {
    const int im = net1k + IM_USER;

    const unsigned int getType = im + 1;
    const unsigned int uiEvent = im + 2;
    const unsigned int registerPlug = im + 3;
    const unsigned int unregisterPlug = im + 4;

    const unsigned int add = im + 10;
    const unsigned int remove = im + 11;
    const unsigned int query = im + 12;
    const unsigned int get = im + 13; ///< je¿eli p2 = true id jest identyfikatorem elementu w tablicy 
    const unsigned int has = im + 14;
    const unsigned int count = im + 15;
    const unsigned int refresh = im + 16;
  };
};

#endif /*__KTRANSFER3_H__*/