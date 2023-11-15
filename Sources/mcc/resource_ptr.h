#ifndef MCC_RESOURCE_PTR_H
#define MCC_RESOURCE_PTR_H

#include "mcc/platform.h"

namespace mcc::resource {
  class Pointer {
  private:
    uword address_;
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