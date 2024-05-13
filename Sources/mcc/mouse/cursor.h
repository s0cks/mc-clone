#ifndef MCC_CURSOR_H
#define MCC_CURSOR_H

#include <string>

namespace mcc::mouse {
  class Cursor {
  protected:
    Cursor() = default;
  public:
    virtual ~Cursor() = default;
    virtual std::string ToString() const = 0;
  };

  Cursor* GetCursor();
  void SetCursor(Cursor* cursor);
}

#include "mcc/mouse/cursor_glfw.h"

#endif //MCC_CURSOR_H