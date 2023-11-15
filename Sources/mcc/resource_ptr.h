#ifndef MCC_RESOURCE_PTR_H
#define MCC_RESOURCE_PTR_H

#include "mcc/platform.h"
#include "mcc/relaxed_atomic.h"

namespace mcc::resource {
  class Pointer {
  private:
    RelaxedAtomic<uword> tag_;
  public:
    Pointer(const uword address):
      address_(address) {
    }
    ~Pointer() = default;

    uword address() const {
      return address_;
    }
  };
}

#endif //MCC_RESOURCE_PTR_H