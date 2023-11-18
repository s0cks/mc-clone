#ifndef MCC_RESOURCE_PTR_H
#define MCC_RESOURCE_PTR_H

#include "mcc/platform.h"
#include "mcc/relaxed_atomic.h"

#include "mcc/resource_tag.h"

namespace mcc::resource {
  class Pointer {
    template<typename T>
    friend class Reference;
  private:
    Tag tag_;
    RelaxedAtomic<uword> references_;
    RelaxedAtomic<uword> data_;

    Pointer() = default;
    Pointer(const Tag tag, const uword data):
      tag_(tag),
      data_(data) {
    }

    inline void IncrementReferenceCounter() {
      references_ += 1;
    }

    inline void DecrementReferenceCounter() {
      references_ -= 1;
    }
  public:
    ~Pointer() = default;

    Tag GetTag() const {
      return (Tag)tag_;
    }

    uword GetDataAddress() const {
      return (uword)data_;
    }

    friend std::ostream& operator<<(std::ostream& stream, const Pointer& rhs) {
      stream << "Pointer(";
      stream << "tag=" << rhs.GetTag() << ", ";
      stream << "address=" << rhs.GetDataAddress();
      stream << ")";
      return stream;
    }
  public:
    static inline uword
    New(const Tag& tag, const uword data) {
      return (uword) new Pointer(tag, data);
    }
  };
}

#endif //MCC_RESOURCE_PTR_H