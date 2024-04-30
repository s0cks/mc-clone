#ifndef MCC_UBO_SCOPE_H
#error "Please #include <mcc/ubo/ubo_scope.h> instead."
#endif //MCC_UBO_SCOPE_H

#ifndef MCC_UBO_SCOPE_BIND_H
#define MCC_UBO_SCOPE_BIND_H

#include "mcc/gfx_bind_scope.h"
#include "mcc/ubo/ubo_scope.h"

namespace mcc::ubo {
  class UboBindScope : public UboScope,
                       public gfx::BindScope {
  public:
    explicit UboBindScope(Ubo* ubo):
      UboScope(ubo) {
      Bind();
    }
    ~UboBindScope() override {
      Unbind();
    }

    void Bind() const override;
    void Unbind() const override;
  };
}

#endif //MCC_UBO_SCOPE_BIND_H