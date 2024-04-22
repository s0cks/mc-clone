#ifndef MCC_UV_UTILS_H
#define MCC_UV_UTILS_H

#include <uv.h>
#include <functional>
#include <glog/logging.h>

#include "mcc/common.h"
#include "mcc/uv/uv_status.h"

namespace mcc::uv {
#define CHECK_UV_RESULT(Severity, Result, Message) \
  LOG_IF(Severity, (Result) != UV_OK) << (Message) << ": " << uv_strerror((Result));

  class Loop {
  public:
    enum RunMode {
      kRunDefault = UV_RUN_DEFAULT,
      kRunNoWait = UV_RUN_NOWAIT,
      kRunOnce = UV_RUN_ONCE,
    };
  protected:
    uv_loop_t* loop_;

    Loop(uv_loop_t* loop):
      loop_(loop) {
    }
  public:
    virtual ~Loop() = default;

    uv_loop_t* GetLoop() const {
      return loop_;
    }

    void Run(const RunMode mode = kRunDefault) {
      VLOG(3) << "running loop.";
      const auto result = uv_run(GetLoop(), static_cast<uv_run_mode>(mode));
      LOG_IF(ERROR, result == UV_OK) << "failed to run loop: " << uv_strerror(result);
      VLOG(3) << "done running loop.";
    }
  };

  static inline uint64_t
  Now() {
    return uv_hrtime();
  }
}

#endif //MCC_UV_UTILS_H