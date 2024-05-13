#ifndef MCC_CURSOR_H
#error "Please #include <mcc/mouse/cursor.h> instead."
#endif //MCC_CURSOR_H

#ifndef MCC_CURSOR_GLFW_H
#define MCC_CURSOR_GLFW_H

#include "mcc/gfx.h"
#include "mcc/mouse/cursor.h"

namespace mcc::mouse {
  typedef GLFWcursor CursorHandle;

  class GlfwCursor : public Cursor {
    friend class Cursor;
  public:
    enum Shape : int {
      kArrow        = GLFW_ARROW_CURSOR,
      kIBeam        = GLFW_IBEAM_CURSOR,
      kCrosshair    = GLFW_CROSSHAIR_CURSOR,
      kHand         = GLFW_HAND_CURSOR,
      kHResize      = GLFW_HRESIZE_CURSOR,
      kVResize      = GLFW_VRESIZE_CURSOR,
      kTotalNumberOfShapes,
      kDefaultShape = kArrow,
    };
  protected:
    CursorHandle* handle_;

    explicit GlfwCursor(CursorHandle* handle):
      Cursor(),
      handle_(handle) {
      MCC_ASSERT(handle);
    }

    inline CursorHandle* handle() const {
      return handle_;
    }
  public:
    ~GlfwCursor() override = default;
    std::string ToString() const override;
  private:
    static GlfwCursor* CreateDefaultCursor(const Shape shape = kDefaultShape);
  public:
    static Cursor* GetDefaultCursor();
  };
}

#endif //MCC_CURSOR_GLFW_H