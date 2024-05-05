#include "mcc/color.h"

#include "mcc/gfx.h"

namespace mcc {
  namespace gfx {
    void SetClearColor(const Color& c) {
      glClearColor(c[0], c[1], c[2], c[3]);
      CHECK_GL(FATAL);
    }
  }
}