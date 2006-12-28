#ifndef __KTRANSFER3_H__
#define __KTRANSFER3_H__

namespace kTransfer3 {
  const UINT net = 130;
  const UINT net1000 = 130 * 1000;

  namespace cfg {
    const unsigned int cfg = net1000 + 500;

  };

  namespace ui {
    const unsigned int ui = net1000;

    const unsigned int cfgGroup = ui + 1;
  };

  namespace act {
  };

  namespace im {
    const int im = net1000 + IM_USER;

    const unsigned int transferGetType = im + 1;
    const unsigned int transferUiEvent = im + 2;
    const unsigned int transferRegisterPlug = im + 3;
    const unsigned int transferDeletePlug = im + 4;
  };

  namespace imi {
    const int imi = net1000 + IM_USER + 200;

    const unsigned int transferAdd = imi + 1;
    const unsigned int transferDelete = imi + 2;
    const unsigned int transferQuery = imi + 3;
    const unsigned int transferGet = imi + 4; ///< je¿eli p2 = true id jest identyfikatorem elementu w tablicy 
    const unsigned int transferIs = imi + 5;
    const unsigned int transferCount = imi + 6;
    const unsigned int transferRefresh = imi + 7;
  };
};

#endif /*__KTRANSFER3_H__*/