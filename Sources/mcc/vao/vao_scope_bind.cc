#include "mcc/vao/vao_scope.h"
#include "mcc/vao/vao.h"

namespace mcc::vao {
  void VaoBindScope::Bind() const {
    Vao::BindVao(GetVaoId());
  }

  void VaoBindScope::Unbind() const {
    Vao::UnbindVao();
  }
}