#ifndef MCC_VAO_SCOPE_H
#define MCC_VAO_SCOPE_H

#include "mcc/vao/vao_id.h"
#include "mcc/gfx_bind_scope.h"

namespace mcc {
  namespace vao {
    class Vao;
    class VaoScope {
    protected:
      Vao* vao_;

      explicit VaoScope(Vao* vao):
        vao_(vao) {
      } 
    public:
      virtual ~VaoScope() = default;

      Vao* GetVao() const {
        return vao_;
      }

      VaoId GetVaoId() const;
    };

    class VaoBindScope : public VaoScope,
                         public gfx::BindScope {
    public:
      explicit VaoBindScope(Vao* vao):
        VaoScope(vao),
        gfx::BindScope() {
      }

      void Bind() const override;
      void Unbind() const override;
    };
  }
  using vao::VaoBindScope;
}

#endif //MCC_VAO_SCOPE_H