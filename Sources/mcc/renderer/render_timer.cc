#include "mcc/renderer/render_timer.h"

#include "mcc/renderer/renderer.h"

namespace mcc::render {
  void RenderTimer::OnIdle(uv_idle_t* handle) {
    const auto timer = GetTimer(handle);
    const auto now_ns = uv_hrtime();
    const auto last_ns = renderer::Renderer::GetLastFrameTimeInNanoseconds();
    const auto delta_ns = (now_ns - last_ns);
    if((delta_ns / NSEC_PER_SEC) < timer->GetRateLimit())
      return;
    DLOG(INFO) << "scheduling render....";
    timer->tick_ = {
      .ts = now_ns,
      .last = last_ns,
    };
    timer->Send();
  }
}