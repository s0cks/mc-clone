#ifndef MCC_IBO_SCOPE_H
#error "Please #include <mcc/ibo/ibo_scope.h> instead."
#endif //MCC_IBO_SCOPE_H

#ifndef MCC_IBO_DRAW_SCOPE_H
#define MCC_IBO_DRAW_SCOPE_H

#include "mcc/ibo/ibo_scope_bind.h"
#include "mcc/gfx_draw_mode.h"

namespace mcc::ibo {
  class IboDrawScope : public IboBindScope {
  public:
    explicit IboDrawScope(Ibo* ibo):
      IboBindScope(ibo) {
    }
    ~IboDrawScope() override = default;

    void Draw(const gfx::DrawMode mode, const uint64_t count, const GLvoid* indices = 0);
    void Draw(const gfx::DrawMode mode);
  };
}

#endif //MCC_IBO_DRAW_SCOPE_H