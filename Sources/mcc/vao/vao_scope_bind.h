#ifndef MCC_VAO_SCOPE_H
#error "Please #include <mcc/vao/vao_scope.h> instead."
#endif //MCC_VAO_SCOPE_H

#ifndef MCC_VAO_SCOPE_BIND_H
#define MCC_VAO_SCOPE_BIND_H

#include "mcc/vao/vao_scope.h"
#include "mcc/gfx_bind_scope.h"

namespace mcc::vao {
  class VaoBindScope : public VaoScope,
                        public gfx::BindScope {
  public:
    explicit VaoBindScope(Vao* vao):
      VaoScope(vao),
      gfx::BindScope() {
      Bind();
    }
    ~VaoBindScope() override {
      Unbind();
    }

    void Bind() const override;
    void Unbind() const override;
  };
}

#endif //MCC_VAO_SCOPE_BIND_H