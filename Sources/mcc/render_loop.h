#ifndef MCC_RENDER_LOOP_H
#define MCC_RENDER_LOOP_H

#include <uv.h>
#include <glog/logging.h>

#include "mcc/common.h"
#include "mcc/uv_utils.h"

namespace mcc {
  typedef std::function<void()> RenderCallback;

  class RenderLoop {
  public:
    enum RunMode {
      kDefault = UV_RUN_DEFAULT,
      kOnce = UV_RUN_ONCE,
      kNoWait = UV_RUN_NOWAIT,
    };
  protected:
    uv_loop_t* loop_;

    RenderLoop(uv_loop_t* loop):
      loop_(loop) {
    }

    static void SetRenderLoop(RenderLoop* value);
  public:
    virtual ~RenderLoop() = default;
    DEFINE_NON_COPYABLE_TYPE(RenderLoop);

    uv_loop_t* loop() const {
      return loop_;
    }

    void Run(const RunMode mode = kDefault) {
      VLOG(3) << "running event loop.";
      const auto result = uv_run(loop(), static_cast<uv_run_mode>(mode));
      LOG_IF(ERROR, result == UV_OK) << "failed to run render loop: " << uv_strerror(result);
      VLOG(3) << "done running event loop.";
    }

    friend std::ostream& operator<<(std::ostream& stream, const RenderLoop& rhs) {
      stream << "RenderLoop(";
      stream << "loop=" << ((void*) rhs.loop());
      stream << ")";
      return stream;
    }
  public:
    static RenderLoop* Initialize(uv_loop_t* loop = uv_loop_new());
    static RenderLoop* GetRenderLoop();
  };

  typedef uv::IdleCallback PreRenderCallback;
  typedef uv::IdleHandle PreRenderHandle;

  typedef uv::PrepareCallback RenderCallback;
  typedef uv::PrepareHandle RenderHandle;

  typedef uv::CheckCallback PostRenderCallback;
  typedef uv::CheckHandle PostRenderHandle;
}

#endif //MCC_RENDER_LOOP_H