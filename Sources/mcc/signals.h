#ifndef MCC_SIGNALS_H
#define MCC_SIGNALS_H

#include "mcc/common.h"
#ifdef OS_IS_OSX

#include <signal.h>

#endif //OS_IS_OSX

namespace mcc {
  namespace signal {
    enum Signal : int {
      kNone = 0,
      kAbort = SIGABRT,
      kFpError = SIGFPE,
      kIllegalInstr = SIGILL,
      kInterrupt = SIGINT,
      kSegfault = SIGSEGV,
      kTermination = SIGTERM,
    };
  }
}

#endif //MCC_SIGNALS_H