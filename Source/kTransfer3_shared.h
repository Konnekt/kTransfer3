#ifndef __KTRANSFER3_SHARED_H__
#define __KTRANSFER3_SHARED_H__

#include <konnekt/plug_export.h>
#include <konnekt/ui.h>
#include <konnekt/plug_func.h>
#include <konnekt/plug.h>
#include <konnekt/core_ctrl.h>

namespace kTransfer3 {
  UINT kt3_net = 130; 

  inline bool isKT3PlugExist() {
    return Ctrl->ICMessage(IMC_FINDPLUG, kt3_net, IMT_ALL);
  }

  inline bool isKT3PlugExistAndPlugOut() {
    if (Ctrl->ICMessage(IMC_FINDPLUG, kt3_net, IMT_ALL)) {
      Ctrl->IMessage (&sIMessage_plugOut (Ctrl->ID(), "Wtyczka nie dzia³a bez uruchomionej wtyczki kTransfer3", sIMessage_plugOut::erYes, sIMessage_plugOut::euNowAndOnNextStart));
       return false;
    }
    return true;
  }
};

#endif /*__KTRANSFER3_SHARED_H__*/