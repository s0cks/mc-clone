#ifndef MCC_SIGNALS_H
#define MCC_SIGNALS_H

#include "mcc/common.h"
#if defined(OS_IS_OSX) || defined(OS_IS_LINUX)
#include <csignal>
#endif //OS_IS_OSX

namespace mcc {
  enum Signal : int {
    kNone = 0,
    kAbort = SIGABRT,
    kFpError = SIGFPE,
    kIllegalInstr = SIGILL,
    kInterrupt = SIGINT,
    kSegfault = SIGSEGV,
    kTerminate = SIGTERM,
  };

  void InitSignalHandlers();
}

#endif //MCC_SIGNALS_H