#ifndef MCC_CURSOR_H
#define MCC_CURSOR_H

#include <string>
#include <glog/logging.h>
#include "mcc/rx.h"
#include "mcc/uri.h"

namespace mcc::mouse {
  class Cursor {
  protected:
    Cursor() = default;
  public:
    virtual ~Cursor() = default;
    virtual std::string ToString() const = 0;
  };

  rx::observable<std::string> ListAvailableCursors();

  void SetDefaultCursor(Cursor* cursor);
  void SetDefaultCursor(const uri::Uri& uri);
  Cursor* GetDefaultCursor();

  static inline void
  SetDefaultCursor(const uri::basic_uri& uri) {
    if(!(StartsWith(uri, "file:") && EndsWith(uri, ".png"))
    && !StartsWith(uri, "cursor:")) {
      LOG(WARNING) << "invalid Cursor Uri: " << uri;
      return SetDefaultCursor(uri::Uri(fmt::format("cursor://{0:s}", uri)));
    }
    return SetDefaultCursor(uri::Uri(uri));
  }

  void SetCurrentCursor(Cursor* cursor);
  void SetCurrentCursor(const uri::Uri& uri);
  Cursor* GetCurrentCursor();

  static inline void
  SetCurrentCursor(const uri::basic_uri& uri) {
    if(!(StartsWith(uri, "file:") && EndsWith(uri, ".png"))
    && !StartsWith(uri, "cursor:")) {
      LOG(WARNING) << "invalid Cursor Uri: " << uri;
      return SetCurrentCursor(uri::Uri(fmt::format("cursor://{0:s}", uri)));
    }
    return SetCurrentCursor(uri::Uri(uri));
  }
}

#include "mcc/mouse/cursor_glfw.h"

#endif //MCC_CURSOR_H