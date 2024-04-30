#include "mcc/ibo/ibo_scope.h"
#include "mcc/ibo/ibo.h"

namespace mcc::ibo {
  void IboBindScope::Bind() const {
    return Ibo::BindIbo(GetIboId());
  }

  void IboBindScope::Unbind() const {
    return Ibo::BindDefaultIbo();
  }
}