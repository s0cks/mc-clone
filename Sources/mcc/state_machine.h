#ifndef MCC_STATE_MACHINE_H
#define MCC_STATE_MACHINE_H

#include "mcc/common.h"

namespace mcc {

#define DECLARE_STATE_MACHINE(Name)                   \
  DEFINE_NON_INSTANTIABLE_TYPE(Name);                 \
  private:                                            \
    static void SetState(const Name##State state);    \
  public:                                             \
    static Name##State GetState();
}

#endif //MCC_STATE_MACHINE_H