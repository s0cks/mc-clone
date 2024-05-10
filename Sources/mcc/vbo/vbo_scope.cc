#include "mcc/vbo/vbo_scope.h"
#include "mcc/vbo/vbo.h"

namespace mcc::vbo {
  VboId VboScope::GetVboId() const {
    return GetVbo()->GetId();
  }

  MappedVboScope::MappedVboScope(const gfx::Access access, Vbo* vbo):
    gfx::MappedBufferScope(GL_ARRAY_BUFFER, access, vbo->GetSize()) {
  }
}