#ifndef MCC_RESOURCE_H
#error "Please #include <mcc/resource.h> instead"
#endif //MCC_RESOURCE_H

#ifndef MCC_RESOURCE_REFERENCE_H
#define MCC_RESOURCE_REFERENCE_H

#include "mcc/flags.h"
#include "mcc/resource_ptr.h"
#include "mcc/resource_tag.h"

namespace mcc::resource {
  template<typename T>
  class Reference {
  private:
    Pointer ptr_;
  public:
    Reference():
      ptr_() {
    }
    explicit Reference(const Pointer ptr):
      ptr_(ptr) {
    }
    Reference(const Reference& rhs) = default;
    ~Reference() = default;

    bool valid() const {
      return ptr_.GetAddress() != 0;
    }

    Pointer ptr() const {
      return ptr_;
    }

    T* operator->() const {
      return reinterpret_cast<T*>(ptr_.GetAddress());
    }

    Reference& operator=(const Reference& rhs) = default;

    friend std::ostream& operator<<(std::ostream& stream, const Reference& rhs) {
      stream << "resource::Reference(";
      stream << "ptr=" << rhs.ptr_;
      stream << ")";
      return stream;
    }
  };
}

#endif //MCC_RESOURCE_REFERENCE_H