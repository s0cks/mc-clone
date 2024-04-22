#include "mcc/renderer/render_timer.h"

#include "mcc/renderer/renderer.h"

namespace mcc::render {
  RenderTimer::RenderTimer(const Renderer* renderer,
                           const FramesPerSecond target,
                           const OnTick& callback):
    renderer_(renderer),
    idle_(),
    target_(target),
    rate_limit_(CalculateRateLimit(target)),
    callback_(this, callback) {
    SetTimer(idle_, this);
    {
      const auto err = uv_idle_init(GetLoop(), &idle_);
      LOG_IF(ERROR, err != UV_OK) << "failed to initialize RenderTimer handle: " << uv_strerror(err);
    }
    {
      const auto err = uv_idle_start(&idle_, &OnIdle);
      LOG_IF(ERROR, err != UV_OK) << "failed to start RenderTimer handle: " << uv_strerror(err);
    }
  }

  RenderTimer::~RenderTimer() {
    const auto err = uv_idle_stop(&idle_);
    LOG_IF(ERROR, err != UV_OK) << "failed to stop RenderTimer handle: " << uv_strerror(err);
  }

  uv_loop_t* RenderTimer::GetLoop() const {
    return GetRenderer()->GetLoop();
  }

  void RenderTimer::OnIdle(uv_idle_t* handle) {
    const auto timer = GetTimer(handle);
    const auto now_ns = uv::Now();
    const auto last_ns = timer->GetRenderer()->GetNanosSinceLastFrame(now_ns);
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