#ifndef MCC_RENDER_LOOP_H
#define MCC_RENDER_LOOP_H

#include <uv.h>
#include <glog/logging.h>

#include "mcc/common.h"
#include "mcc/fixed_rate_loop.h"

namespace mcc {
  typedef std::function<void()> RenderCallback;

  class RenderLoop : public FixedRateLoop {
  public:
    enum RunMode {
      kDefault = UV_RUN_DEFAULT,
      kOnce = UV_RUN_ONCE,
      kNoWait = UV_RUN_NOWAIT,
    };
  protected:
    RenderLoop(uv_loop_t* loop):
      FixedRateLoop(loop) {
    }

    static void SetRenderLoop(RenderLoop* value);
  public:
    virtual ~RenderLoop() = default;
    DEFINE_NON_COPYABLE_TYPE(RenderLoop);

    void Run(const RunMode mode = kDefault) {
      VLOG(3) << "running event loop.";
      const auto result = uv_run(GetLoop(), static_cast<uv_run_mode>(mode));
      LOG_IF(ERROR, result == UV_OK) << "failed to run render loop: " << uv_strerror(result);
      VLOG(3) << "done running event loop.";
    }

    friend std::ostream& operator<<(std::ostream& stream, const RenderLoop& rhs) {
      stream << "RenderLoop(";
      stream << "loop=" << ((void*) rhs.GetLoop());
      stream << ")";
      return stream;
    }
  public:
    static RenderLoop* Initialize(uv_loop_t* loop = uv_loop_new());
    static RenderLoop* GetRenderLoop();
  };

  typedef uv::IdleCallback PreRenderCallback;

  class PreRenderHandle : public uv::IdleHandle {
  public:
    PreRenderHandle(RenderLoop* loop, PreRenderCallback callback):
      uv::IdleHandle(loop->GetLoop(), callback) {
    }
    ~PreRenderHandle() override = default;
  };

  typedef uv::PrepareCallback RenderCallback;
  class RenderHandle : public uv::PrepareHandle {
  public:
    RenderHandle(RenderLoop* loop, RenderCallback callback):
      uv::PrepareHandle(loop->GetLoop(), callback) {
    }
    ~RenderHandle() override = default;
  };

  typedef uv::CheckCallback PostRenderCallback;
  class PostRenderHandle : public uv::CheckHandle {
  public:
    PostRenderHandle(RenderLoop* loop, PostRenderCallback callback):
      uv::CheckHandle(loop->GetLoop(), callback) {
    }
    ~PostRenderHandle() override = default;
  };
}

#endif //MCC_RENDER_LOOP_H