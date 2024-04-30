#include "mcc/vao/vao_scope.h"
#include "mcc/vao/vao.h"

namespace mcc::vao {
  VaoId VaoScope::GetVaoId() const {
    return GetVao()->GetId();
  }

  void VaoBindScope::Bind() const {
    Vao::BindVao(GetVaoId());
  }

  void VaoBindScope::Unbind() const {
    Vao::UnbindVao();
  }
}