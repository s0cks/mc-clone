#include "mcc/gfx_mapped_buffer_scope.h"

namespace mcc::gfx {
  uword MappedBufferScope::MapBuffer(const GLenum target, const Access access) {
    const auto ptr = glMapBuffer(target, access);
    CHECK_GL(FATAL);
    return (uword) ptr;
  }

  void MappedBufferScope::UnmapBuffer(const GLenum target) {
    glUnmapBuffer(target);
    CHECK_GL(FATAL);
  }
}