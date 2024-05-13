#include "mcc/mouse/cursor.h"

#include <sstream>
#include "mcc/thread_local.h"

namespace mcc::mouse {
  static ThreadLocal<GlfwCursor> default_cursor_;

  std::string GlfwCursor::ToString() const {
    std::stringstream ss;
    ss << "GlfwCursor(";
    ss << "handle=" << handle();
    ss << ")";
    return ss.str();
  }

  GlfwCursor* GlfwCursor::CreateDefaultCursor(const Shape shape) {
    const auto handle = glfwCreateStandardCursor(shape);
    MCC_ASSERT(handle);
    return new GlfwCursor(handle);
  }

  Cursor* GlfwCursor::GetDefaultCursor() {
    if(default_cursor_)
      return default_cursor_.Get();
    const auto cursor = CreateDefaultCursor();
    default_cursor_.Set(cursor);
    return cursor;
  }
}