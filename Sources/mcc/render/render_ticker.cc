#include "mcc/render/render_ticker.h"
#include "mcc/render/renderer.h"

namespace mcc::render {
  void RenderTicker::OnPreTick() {
    Renderer::Publish<PreRenderEvent>();
  }

  void RenderTicker::OnTick(const uv::Tick& tick) {
    const auto renderer = Renderer::Get();
    MCC_ASSERT(renderer);
    renderer->Schedule();
  }

  void RenderTicker::OnPostTick(const uv::Tick& tick) {
    Renderer::Publish<PostRenderEvent>(tick);
  }
}