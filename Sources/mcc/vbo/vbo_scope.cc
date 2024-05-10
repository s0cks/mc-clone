#include "mcc/vbo/vbo_scope.h"
#include "mcc/vbo/vbo.h"

namespace mcc::vbo {
  VboId VboScope::GetVboId() const {
    return GetVbo()->GetId();
  }

  WriteOnlyVboScope::WriteOnlyVboScope(Vbo* vbo):
    gfx::WriteOnlyMappedBufferScope<GL_ARRAY_BUFFER>(vbo->GetSize()) { //TODO: use Vbo::kGlType
  }

  ReadOnlyVboScope::ReadOnlyVboScope(Vbo* vbo):
    gfx::ReadOnlyMappedBufferScope<GL_ARRAY_BUFFER>(vbo->GetSize()) { //TODO: use Vbo::kGlType
  }
}