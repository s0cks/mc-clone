#include "mcc/ibo/ibo_scope.h"
#include "mcc/ibo/ibo.h"

namespace mcc::ibo {
  IboId IboScope::GetIboId() const {
    return GetIbo()->GetId();
  }
}