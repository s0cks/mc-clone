#ifndef MCC_UBO_SCOPE_H
#error "Please #include <mcc/ubo/ubo_scope.h> instead."
#endif //MCC_UBO_SCOPE_H

#ifndef MCC_UBO_SCOPE_READ_H
#define MCC_UBO_SCOPE_READ_H

#include "mcc/ubo/ubo_scope.h"

namespace mcc::ubo {
  template<typename T>
  class UboReadScope : public UboMappedScope {
  public:
    explicit UboReadScope(Ubo* ubo):
      UboMappedScope(ubo, kReadOnly) {
    }
    ~UboReadScope() override = default;

    rx::observable<T> ReadAll() const {
      if(!IsMapped())
        return rx::observable<>::empty<T>();
        
      return rx::observable<>::create<T>([this](rx::subscriber<T> s) {
        auto current = GetStartingAddress();
        while(current < GetEndingAddress()) {
          const auto v = (T*)current;
          s.on_next(*v);
          current += sizeof(T);
        }
        s.on_completed();
      });
    }
  };
}

#endif //MCC_UBO_SCOPE_READ_H