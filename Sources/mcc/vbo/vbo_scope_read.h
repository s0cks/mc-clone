#ifndef MCC_VBO_SCOPE_H
#error "Please #include <mcc/vbo/vbo_scope.h> instead."
#endif //MCC_VBO_SCOPE_H

#ifndef MCC_VBO_SCOPE_READ_H
#define MCC_VBO_SCOPE_READ_H

#include "mcc/vbo/vbo_scope.h"
#include "mcc/gfx_mapped_buffer_scope.h"

namespace mcc::vbo {
  template<class V>
  class VboReadScope : public VboBindScope,
                       public ReadOnlyVboScope {
  public:
    explicit VboReadScope(Vbo* vbo):
      VboBindScope(vbo),
      ReadOnlyVboScope(vbo) {
    }
    ~VboReadScope() override = default;

    rx::observable<V> ReadAll() const {
      if(!IsMapped())
        return rx::observable<>::empty<V>();
        
      return rx::observable<>::create<V>([this](rx::subscriber<V> s) {
        Iterator<V> iter(this);
        while(iter.HasNext()) {
          const auto next = iter.Next();
          s.on_next(next);
        }
        s.on_completed();
      });
    }
  };
}

#endif //MCC_VBO_SCOPE_READ_H