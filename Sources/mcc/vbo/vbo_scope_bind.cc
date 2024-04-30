#include "mcc/vbo/vbo_scope.h"
#include "mcc/vbo/vbo.h"

namespace mcc::vbo {
  void VboBindScope::Bind() const {
    Vbo::BindVbo(GetVboId());
  }

  void VboBindScope::Unbind() const {
    Vbo::BindDefaultVbo();
  }
}