#include "mcc/ubo/ubo_scope.h"
#include "mcc/ubo/ubo.h"

namespace mcc::ubo {
  void UboBindScope::Bind() const {
    Ubo::BindUbo(GetUboId());
  }

  void UboBindScope::Unbind() const {
    Ubo::UnbindUbo();
  }
}