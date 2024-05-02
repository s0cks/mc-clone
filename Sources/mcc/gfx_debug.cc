#include "mcc/gfx_debug.h"
#include "mcc/gfx.h"

namespace mcc::gfx {
#ifndef MCC_DEBUG
  DEFINE_bool(enable_gfx_debug, false, "Enable OpenGL debugging.");
#endif //MCC_DEBUG

  void EnableDebug() {
#ifndef MCC_DEBUG
    if(!IsDebugEnabled())
      return;
#endif //MCC_DEBUG
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
  }

  bool IsDebugEnabled() {
#ifdef MCC_DEBUG
    return true;
#else
    return FLAGS_enable_gfx_debug;
#endif //MCC_DEBUG
  }
}