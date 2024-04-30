#ifndef MCC_IBO_SCOPE_H
#error "Please #include <mcc/ibo/ibo_scope.h> instead."
#endif //MCC_IBO_SCOPE_H

#ifndef MCC_IBO_BIND_SCOPE_H
#define MCC_IBO_BIND_SCOPE_H

#include "mcc/ibo/ibo_scope.h"
#include "mcc/gfx_bind_scope.h"

namespace mcc::ibo {
  class IboBindScope : public IboScope,
                       public gfx::BindScope {
  public:
    explicit IboBindScope(Ibo* ibo):
      IboScope(ibo) {
      Bind();
    }
    ~IboBindScope() override {
      Unbind();
    }

    void Bind() const override;
    void Unbind() const override;
  };
}

#endif //MCC_IBO_BIND_SCOPE_H