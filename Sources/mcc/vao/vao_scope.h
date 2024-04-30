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
  }
}

#include "mcc/vao/vao_scope_bind.h"

#endif //MCC_VAO_SCOPE_H