#include "mcc/ibo/ibo_scope.h"
#include "mcc/ibo/ibo.h"

namespace mcc::ibo {
  IboId IboScope::GetIboId() const {
    return GetIbo()->GetId();
  }

  MappedIboScope::MappedIboScope(const gfx::Access access, Ibo* ibo):
    gfx::MappedBufferScope(GL_ELEMENT_ARRAY_BUFFER, access, ibo->GetSize()) {
  }
}