#include "mcc/ibo/ibo_scope.h"
#include "mcc/ibo/ibo.h"

namespace mcc::ibo {
  void IboDrawScope::Draw(const gfx::DrawMode mode, const uint64_t count, const GLvoid* indices) {
    glDrawElements(mode, count, GetIbo()->GetType(), indices);
    CHECK_GL(FATAL);
  }

  void IboDrawScope::Draw(const gfx::DrawMode mode) {
    return Draw(mode, GetIbo()->GetLength());
  }
}