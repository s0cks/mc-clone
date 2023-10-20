#ifndef MCC_RENDER_LOOP_H
#define MCC_RENDER_LOOP_H

#include <uv.h>
#include <glog/logging.h>

#include "mcc/common.h"
#include "mcc/fixed_rate_loop.h"

namespace mcc {
  typedef std::function<void()> RenderCallback;

  class RenderLoop : public FixedRateLoop {
  protected:
    RenderLoop(uv_loop_t* loop);

    static void SetRenderLoop(RenderLoop* value);
  public:
    virtual ~RenderLoop() = default;
    DEFINE_NON_COPYABLE_TYPE(RenderLoop);

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