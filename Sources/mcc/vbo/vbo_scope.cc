#include "mcc/vbo/vbo_scope.h"

namespace mcc::vbo {
  void VboDrawScope::Draw(const GLenum mode, const int64_t first, const int64_t count) const {
    DLOG(INFO) << "drawing " << count << " vertices starting @" << first;
    glDrawArrays(mode, first, count);
    CHECK_GL(FATAL);
  }
}