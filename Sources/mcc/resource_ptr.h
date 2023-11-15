#ifndef MCC_RESOURCE_PTR_H
#define MCC_RESOURCE_PTR_H

#include "mcc/platform.h"
#include "mcc/relaxed_atomic.h"

#include "mcc/resource_tag.h"

namespace mcc::resource {
  class Pointer {
  private:
    Tag tag_;
    RelaxedAtomic<uword> address_;
    RelaxedAtomic<uword> forwarding_;
  public:
    Pointer() = default;
    Pointer(const Tag tag, const uword address):
      tag_(tag),
      address_(address),
      forwarding_(0) {
    }
    ~Pointer() = default;

    Tag GetTag() const {
      return (Tag)tag_;
    }

    uword GetAddress() const {
      return (uword)address_;
    }

    uword GetForwardingAddress() const {
      return (uword)forwarding_;
    }

    friend std::ostream& operator<<(std::ostream& stream, const Pointer& rhs) {
      stream << "Pointer(";
      stream << "address=" << rhs.GetAddress();
      stream << ")";
      return stream;
    }
  };
}

#endif //MCC_RESOURCE_PTR_H