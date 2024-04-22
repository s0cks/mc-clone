#ifndef MCC_VAO_SCOPE_H
#define MCC_VAO_SCOPE_H

#include "mcc/vao/vao.h"

namespace mcc {
  namespace vao {
    class VaoBindScope {
    private:
      VaoId target_;

      static inline void
      BindVao(const VaoId id) {
        glBindVertexArray(id);
        CHECK_GL(FATAL);
      }

      static inline void
      UnbindVao() {
        return BindVao(kDefaultVaoId);
      }
    public:
      explicit VaoBindScope(const VaoId target):
        target_(target) {
        Bind();
      }
      explicit VaoBindScope(const Vao* vao):
        VaoBindScope(vao->GetId()) {
      }
      ~VaoBindScope() {
        Unbind();
      }

      VaoId GetTargetVaoId() const {
        return target_;
      }

      void Bind() {
        BindVao(GetTargetVaoId());
      }

      void Unbind() {
        UnbindVao();
      }
    };
  }
  using vao::VaoBindScope;
}

#endif //MCC_VAO_SCOPE_H