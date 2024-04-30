#ifndef MCC_FBO_SCOPE_H
#error "Please #include <mcc/fbo/fbo_scope.h> instead."
#endif //MCC_FBO_SCOPE_H

#ifndef MCC_FBO_SCOPE_BIND_H
#define MCC_FBO_SCOPE_BIND_H

#include "mcc/fbo/fbo_scope.h"
#include "mcc/gfx_bind_scope.h"

namespace mcc::fbo {
  class FboBindScope : public FboScope,
                       public gfx::BindScope {
  public:
    explicit FboBindScope(Fbo* fbo):
      FboScope(fbo) {
      Bind();
    }
    ~FboBindScope() override {
      Unbind();
    }

    void Bind() const override;
    void Unbind() const override;
  };
}

#endif //MCC_FBO_SCOPE_BIND_H