#ifndef MCC_VBO_SCOPE_H
#error "Please #include <mcc/vbo/vbo_scope.h> instead."
#endif //MCC_VBO_SCOPE_H

#ifndef MCC_VBO_SCOPE_DRAW_H
#define MCC_VBO_SCOPE_DRAW_H

#include "mcc/gfx_draw_mode.h"
#include "mcc/vbo/vbo_scope.h"

namespace mcc::vbo {
  class VboDrawScope : public VboBindScope {
  public:
    explicit VboDrawScope(Vbo* vbo):
      VboBindScope(vbo) {
    }
    ~VboDrawScope() override = default;
    
    void Draw(const gfx::DrawMode mode, const int64_t first, const int64_t count) const;
    void Draw(const gfx::DrawMode mode, const int64_t first = 0) const;

#define DEFINE_DRAW(Name, _)                                                          \
    inline void Draw##Name(const int64_t first, const int64_t count) const {          \
      return Draw(gfx::k##Name##Mode, first, count);                                  \
    }                                                                                 \
    inline void Draw##Name(const int64_t first = 0) const {                           \
      return Draw(gfx::k##Name##Mode, first);                                         \
    }
    FOR_EACH_DRAW_MODE(DEFINE_DRAW)
#undef DEFINE_DRAW
  };
}

#endif //MCC_VBO_SCOPE_DRAW_H