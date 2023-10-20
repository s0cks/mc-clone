#include "mcc/thread_local.h"
#include "mcc/render_loop.h"
#include "mcc/ecs/coordinator.h"

namespace mcc {
  static ThreadLocal<RenderLoop> instance_;

  RenderLoop::RenderLoop(uv_loop_t* loop):
    FixedRateLoop(loop) {
  }

  RenderLoop* RenderLoop::Initialize(uv_loop_t* loop) {
    const auto instance = new RenderLoop(loop);
    SetRenderLoop(instance);
    return instance;
  }

  RenderLoop* RenderLoop::GetRenderLoop() {
    return instance_.Get();
  }

  void RenderLoop::SetRenderLoop(RenderLoop* value) {
    instance_.Set(value);
  }
}