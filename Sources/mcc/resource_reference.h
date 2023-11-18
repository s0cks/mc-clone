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
    uword ptr_;

    inline void IncrementReferenceCounter() {
      if(!valid())
        return;
      ptr()->IncrementReferenceCounter();
    }

    inline void DecrementReferenceCounter() {
      if(!valid())
        return;
      ptr()->DecrementReferenceCounter();
    }
  public:
    Reference():
      ptr_(0) {
    }
    explicit Reference(const uword ptr):
      ptr_(ptr) {
      IncrementReferenceCounter();
    }
    explicit Reference(const Tag& tag, T* data):
      ptr_(Pointer::New(tag, (uword) data)) {
      DecrementReferenceCounter();
    }
    Reference(const Reference& rhs) = default;
    ~Reference() = default;

    inline bool valid() const {
      return ptr_ != 0;
    }

    inline Pointer* ptr() const {
      return (Pointer*) ptr_;
    }

    T* operator->() const {
      return reinterpret_cast<T*>(ptr()->GetDataAddress());
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