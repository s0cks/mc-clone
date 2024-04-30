#include "mcc/vbo/vbo_scope.h"
#include "mcc/vbo/vbo.h"

namespace mcc::vbo {
  VboId VboScope::GetVboId() const {
    return GetVbo()->GetId();
  }
}