#include "mcc/gui/gui_frame_renderer.h"
#include "mcc/window.h"
#include "mcc/renderer/renderer.h"
#include "mcc/terrain/terrain.h"

namespace mcc::gui {
  using renderer::Renderer;

  RendererFrame::RendererFrame():
    Frame("Renderer", kTop, glm::vec2(0.0f, Window::GetHeight() / 8.0f)) {
  }

  void RendererFrame::Render(nk::Context* ctx) {
    struct nk_vec2 size = nk_widget_size(ctx);

    nk_layout_row_dynamic(ctx, size.y / 2, 3);
    nk_label(ctx, "Mode: ", NK_TEXT_LEFT);
    if(nk_option_label(ctx, "Default", Renderer::GetMode() == Renderer::kDefaultMode))
      Renderer::SetMode(Renderer::kDefaultMode);
    if(nk_option_label(ctx, "Wireframe", Renderer::GetMode() == Renderer::kWireframeMode))
      Renderer::SetMode(Renderer::kWireframeMode);

    nk_layout_row_dynamic(ctx, size.y / 2, 4);
    nk_labelf(ctx, NK_TEXT_LEFT, "Entities: %" PRIu64, renderer::Renderer::GetEntityCounter());
    nk_labelf(ctx, NK_TEXT_LEFT, "FPS: %04llu", renderer::Renderer::GetFPS());
    nk_labelf(ctx, NK_TEXT_LEFT, "Last Frame: %04.02fms", (renderer::Renderer::GetLastFrameTimeInNanoseconds() / (1.0f * NSEC_PER_MSEC)));
  }
}