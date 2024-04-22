#include "mcc/renderer/render_ticker.h"

#include "mcc/renderer/renderer.h"

namespace mcc::render {
  void RenderTicker::OnPreTick() {

  }

  void RenderTicker::OnTick(const uv::Tick& tick) {
    DLOG(INFO) << "tick: " << tick;
    const auto renderer = Renderer::Get();
    MCC_ASSERT(renderer);
    renderer->Schedule();
  }

  void RenderTicker::OnPostTick(const uv::Tick& tick) {
    const auto& duration = GetTickDurationSeries();
    const auto& fps = GetTicksPerSecond();
    using namespace units::time;
    DLOG(INFO) << fps << " stats (avg/min/max): " << nanosecond_t(duration.average()) << ", " << nanosecond_t(duration.min()) << ", " << nanosecond_t(duration.max());
  }
}