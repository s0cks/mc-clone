#ifndef MCC_GFX_DEBUG_H
#define MCC_GFX_DEBUG_H

#include "mcc/flags.h"

namespace mcc {
  namespace gfx {
#ifndef MCC_DEBUG
DECLARE_bool(enable_gfx_debug);
#endif//MCC_DEBUG

    void EnableDebug();
    bool IsDebugEnabled();
  }
}

#endif //MCC_GFX_DEBUG_H