#ifndef MCC_MONITOR_H
#define MCC_MONITOR_H

#include "mcc/common.h"

#include "mcc/window/monitor_glfw.h"

namespace mcc {
  class Monitor {
    DEFINE_NON_COPYABLE_TYPE(Monitor);
  protected:
    MonitorHandle* handle_;
  public:
    explicit Monitor(MonitorHandle* handle = GetDefaultMonitorHandle()):
      handle_(handle) {
    }
    virtual ~Monitor() = default;

    inline MonitorHandle* handle() const {
      return handle_;
    }

    Resolution GetResolution() const {
      return GetMonitorResolution(handle());
    }

    friend std::ostream& operator<<(std::ostream& stream, const Monitor& rhs) {
      stream << "Monitor(";
      stream << "handle=" << rhs.handle();
      stream << ")";
      return stream;
    }
  };
}

#endif //MCC_MONITOR_H