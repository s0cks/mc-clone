#include "mcc/ibo/ibo_scope.h"
#include "mcc/ibo/ibo.h"

namespace mcc::ibo {
  IboId IboScope::GetIboId() const {
    return GetIbo()->GetId();
  }

  ReadOnlyIboScope::ReadOnlyIboScope(Ibo* ibo):
    gfx::ReadOnlyMappedBufferScope<GL_ELEMENT_ARRAY_BUFFER>(ibo->GetSize()) { //TODO: use Ibo::kGlType
  }

  WriteOnlyIboScope::WriteOnlyIboScope(Ibo* ibo):
    gfx::WriteOnlyMappedBufferScope<GL_ELEMENT_ARRAY_BUFFER>(ibo->GetSize()) { //TODO: use Ibo::kGlType
  }
}