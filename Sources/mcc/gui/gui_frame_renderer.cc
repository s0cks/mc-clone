#include "mcc/gui/gui_frame_renderer.h"
#include "mcc/window.h"
#include "mcc/renderer/renderer.h"
#include "mcc/terrain/terrain.h"

#include "mcc/shader/shader.h"
#include "mcc/engine/engine.h"

namespace mcc::gui {
  using renderer::Renderer;

  RendererFrame::RendererFrame():
    Frame("Renderer", kTop, glm::vec2(0.0f, Window::GetHeight() / 8.0f)) {
  }

  void RendererFrame::Render(nk::Context* ctx) {
    struct nk_vec2 size = nk_widget_size(ctx);

    nk_layout_row_dynamic(ctx, size.y / 4, 3);
    nk_label(ctx, "Mode: ", NK_TEXT_LEFT);
    if(nk_option_label(ctx, "Default", Renderer::GetMode() == Renderer::kDefaultMode))
      Renderer::SetMode(Renderer::kDefaultMode);
    if(nk_option_label(ctx, "Wireframe", Renderer::GetMode() == Renderer::kWireframeMode))
      Renderer::SetMode(Renderer::kWireframeMode);
    nk_labelf(ctx, NK_TEXT_LEFT, "Entities: %" PRIu64, renderer::Renderer::GetEntityCounter());
    nk_labelf(ctx, NK_TEXT_LEFT, "Vertices: %" PRIu64, renderer::Renderer::GetVertexCounter());

    {
      nk_layout_row_dynamic(ctx, size.y - (size.y - 4), 5);
      nk_labelf(ctx, NK_TEXT_LEFT, "FPS: %04llu", renderer::Renderer::GetFPS());
      const auto samples = renderer::Renderer::GetSamples();
      nk_labelf(ctx, NK_TEXT_LEFT, "Samples: %" PRIu64, renderer::kNumberOfRendererSamples);
      nk_labelf(ctx, NK_TEXT_LEFT, "Avg: %04.04lfms", samples->GetAvgDuration() / (1.0f * NSEC_PER_MSEC));
      nk_labelf(ctx, NK_TEXT_LEFT, "Min: %04.04lfms", samples->GetMinDuration() / (1.0f * NSEC_PER_MSEC));
      nk_labelf(ctx, NK_TEXT_LEFT, "Max: %04.04lfms", samples->GetMaxDuration() / (1.0f * NSEC_PER_MSEC));
    }

    {
      nk_layout_row_dynamic(ctx, size.y - ((size.y - 4) * 2), 5);
      nk_labelf(ctx, NK_TEXT_LEFT, "TPS: %04llu", Engine::GetTPS());
      const auto samples = Engine::GetSamples();
      nk_labelf(ctx, NK_TEXT_LEFT, "Samples: %" PRIu64, samples->GetSize());
      nk_labelf(ctx, NK_TEXT_LEFT, "Avg: %04.04lfms", samples->GetDurationAvg() / (1.0f * NSEC_PER_MSEC));
      nk_labelf(ctx, NK_TEXT_LEFT, "Min: %04.04lfms", samples->GetDurationMin() / (1.0f * NSEC_PER_MSEC));
      nk_labelf(ctx, NK_TEXT_LEFT, "Max: %04.04lfms", samples->GetDurationMax() / (1.0f * NSEC_PER_MSEC));
    }
  }
}