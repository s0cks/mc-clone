#include "mcc/renderer/renderer_stage.h"
#include "mcc/renderer/renderer.h"
#include "mcc/window.h"
#include "mcc/gui/gui.h"

namespace mcc::renderer {
  class FrameRenderer : public gui::FrameVisitor {
  private:
    nk::Context* ctx_;
  public:
    explicit FrameRenderer(nk::Context* ctx):
      FrameVisitor(),
      ctx_(ctx) {
    }
    ~FrameRenderer() override = default;

    bool Visit(gui::FramePtr frame) override {
      frame->RenderFrame(ctx_);
      return true;
    }
  };

  void RenderGuiStage::Render(const Tick& tick, const glm::mat4& projection, const glm::mat4& view) {
    FrameRenderer frame_renderer(gui::Screen::GetNuklearContext());
    Window::VisitFrames(&frame_renderer);
  }

  void DrawGuiStage::Render(const Tick& tick, const glm::mat4& projection, const glm::mat4& view) {
    DLOG(INFO) << "drawing gui....";
    const auto size = Window::GetSize();
    auto proj = glm::mat4(1.0f);
    proj = glm::ortho(0.0f, size[0], size[1], 0.0f);
    gui::Screen::RenderScreen(proj);
  }
}