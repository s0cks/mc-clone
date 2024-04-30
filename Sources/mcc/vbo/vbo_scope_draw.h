#ifndef MCC_VBO_SCOPE_H
#error "Please #include <mcc/vbo/vbo_scope.h> instead."
#endif //MCC_VBO_SCOPE_H

#ifndef MCC_VBO_SCOPE_DRAW_H
#define MCC_VBO_SCOPE_DRAW_H

#include "mcc/vbo/vbo_scope.h"

namespace mcc::vbo {
  class VboDrawScope : public VboBindScope {
  public:
    explicit VboDrawScope(Vbo* vbo):
      VboBindScope(vbo) {
    }
    ~VboDrawScope() override = default;
    
    void Draw(const GLenum mode, const int64_t first, const int64_t count) const;
    void Draw(const GLenum mode, const int64_t first = 0) const;
  };
}

#endif //MCC_VBO_SCOPE_DRAW_H