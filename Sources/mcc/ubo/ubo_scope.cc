#include "mcc/ubo/ubo_scope.h"
#include "mcc/ubo/ubo.h"

namespace mcc::ubo {
  ReadOnlyUboScope::ReadOnlyUboScope(Ubo* ubo):
    gfx::ReadOnlyMappedBufferScope<GL_UNIFORM_BUFFER>(ubo->GetTotalSize()) { //TODO: use Ubo::kGlTarget
  }

  WriteOnlyUboScope::WriteOnlyUboScope(Ubo* ubo):
    gfx::WriteOnlyMappedBufferScope<GL_UNIFORM_BUFFER>(ubo->GetTotalSize()) { //TODO: use Ubo::kGlTarget
  }
}