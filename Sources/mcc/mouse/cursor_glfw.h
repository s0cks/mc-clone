#ifndef MCC_CURSOR_H
#error "Please #include <mcc/mouse/cursor.h> instead."
#endif //MCC_CURSOR_H

#ifndef MCC_CURSOR_GLFW_H
#define MCC_CURSOR_GLFW_H

#include "mcc/gfx.h"
#include "mcc/image/image.h"
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
  public:
    ~GlfwCursor() override = default;
    std::string ToString() const override;

    inline CursorHandle* handle() const { //TODO: reduce visibility
      return handle_;
    }
  private:
    static inline GlfwCursor*
    New(CursorHandle* handle) {
      MCC_ASSERT(handle);
      return new GlfwCursor(handle);
    }
  public:
    static Cursor* NewStandard(const Shape shape = kDefaultShape);
    static Cursor* New(const img::Image* image);
    static Cursor* New(const uri::Uri& uri);

    static inline Cursor*
    New(const uri::basic_uri& uri) {
      if(!(StartsWith(uri, "file:") && EndsWith(uri, ".png"))
      && !StartsWith(uri, "cursor:")) {
        LOG(WARNING) << "invalid Cursor Uri: " << uri;
        return New(uri::Uri(fmt::format("cursor://{0:s}", uri)));
      }
      return New(uri::Uri(uri));
    }
  };
}

#endif //MCC_CURSOR_GLFW_H