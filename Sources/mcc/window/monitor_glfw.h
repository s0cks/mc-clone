#ifndef MCC_MONITOR_GLFW_H
#define MCC_MONITOR_GLFW_H

#include "mcc/gfx.h"
#include "mcc/resolution.h"
#include "mcc/window/monitor_constants.h"

namespace mcc {
  typedef GLFWmonitor MonitorHandle;

  static inline MonitorHandle*
  GetDefaultMonitorHandle() {
    return glfwGetPrimaryMonitor();
  }

  static inline Resolution
  GetMonitorResolution(MonitorHandle* handle) {
    const auto mode = glfwGetVideoMode(handle);
    return { mode->width, mode->height };
  }
}

#endif //MCC_MONITOR_GLFW_H