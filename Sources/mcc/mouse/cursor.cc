#include "mcc/mouse/cursor.h"
#include "mcc/thread_local.h"

namespace mcc::mouse {
  static ThreadLocal<Cursor> cursor_;

  Cursor* GetCursor() {
    if(cursor_)
      return cursor_.Get();
    const auto cursor = GlfwCursor::GetDefaultCursor();
    cursor_.Set(cursor);
    return cursor;
  }

  void SetCursor(Cursor* cursor) {
    MCC_ASSERT(cursor);
    cursor_.Set(cursor);
  }
}