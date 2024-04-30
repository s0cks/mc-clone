#ifndef MCC_VBO_SCOPE_H
#error "Please #include <mcc/vbo/vbo_scope.h> instead."
#endif //MCC_VBO_SCOPE_H

#ifndef MCC_VBO_SCOPE_BIND_H
#define MCC_VBO_SCOPE_BIND_H

#include "mcc/vbo/vbo_scope.h"
#include "mcc/gfx_bind_scope.h"

namespace mcc::vbo {
  class VboBindScope : public VboScope,
                       public gfx::BindScope {
  public:
    explicit VboBindScope(Vbo* vbo):
      VboScope(vbo) {
      Bind();
    }
    ~VboBindScope() override {
      Unbind();
    }

    void Bind() const override;
    void Unbind() const override;
  };
}

#endif //MCC_VBO_SCOPE_BIND_H