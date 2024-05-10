#ifndef MCC_IBO_SCOPE_H
#error "Please #include <mcc/ibo/ibo_scope.h> instead."
#endif //MCC_IBO_SCOPE_H

#ifndef MCC_IBO_SCOPE_READ_H
#define MCC_IBO_SCOPE_READ_H

#include "mcc/ibo/ibo_scope.h"

namespace mcc::ibo {
  template<typename I>
  class IboReadScope : public IboBindScope,
                       public MappedIboScope {
  public:
    explicit IboReadScope(Ibo* ibo):
      IboBindScope(ibo),
      MappedIboScope(gfx::kReadOnly, ibo) {
    }
    ~IboReadScope() override = default;

    const I* begin() const {
      return (const I*) GetStartingAddressPointer();
    }

    const I* end() const {
      return (const I*) GetEndingAddressPointer();
    }

    rx::observable<I> ToObservable() const {
      if(!IsMapped())
        return rx::observable<>::empty<I>();
      return rx::observable<>::create<I>([this](rx::subscriber<I> s) {
        Iterator<I> iter(this);
        while(iter.HasNext()) {
          s.on_next(iter.Next());
        }
        s.on_completed();
      });
    }
  };
}

#endif //MCC_IBO_SCOPE_READ_H