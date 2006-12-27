#pragma once

#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

#include "stdafx.h"
#include "Transfer.h"

using namespace Stamina;

namespace kTransfer3 {
  class Controller: public PlugController<Controller> {
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
    }

  public:
    ~Controller();
  };
};

#endif /*__CONTROLLER_H__*/