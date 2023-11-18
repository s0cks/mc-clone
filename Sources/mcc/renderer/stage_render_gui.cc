#include "mcc/renderer/stage.h"
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

  RenderScreenStage::RenderScreenStage(uv_loop_t* loop, FrameBuffer* fb):
    RenderStage(loop),
    shader_(GetShader("framebuffer")),
    bloom_(fb, Dimension(Window::GetSize()), GetShader("blur")),
    pipeline_(fb, kColorAndDepthClearMask) {
    pipeline_.AddChild(new ApplyPipeline([this]() {
      auto fb = Renderer::GetFrameBuffer();
      fb->GetColorBufferAttachment(0)->GetTexture()->Bind(0);
      bloom_.GetFrameBuffer(0)->GetColorBufferAttachment(0)->GetTexture()->Bind(1);
    }));
    pipeline_.AddChild(new ApplyShaderPipeline(shader_, [](const ShaderRef& shader) {
      shader->SetInt("tex", 0);
      shader->SetInt("bloomTex", 1);
      shader->SetBool("bloom", true);
      shader->SetBool("hdr", true);
      shader->SetFloat("gamma", 2.2f);
      shader->SetFloat("exposure", 1.0f);
    }));
  }

  RenderScreenStage::RenderScreenStage(uv_loop_t* loop):
    RenderScreenStage(loop, Renderer::GetFrameBuffer()) {
  }

  void RenderGuiStage::Render(const Tick& tick) {
    FrameRenderer frame_renderer(gui::Screen::GetNuklearContext());
    Window::VisitFrames(&frame_renderer);
    const auto size = Window::GetSize();
    auto proj = glm::mat4(1.0f);
    proj = glm::ortho(0.0f, size[0], size[1], 0.0f);
    gui::Screen::RenderScreen(proj);
  }

  void RenderScreenStage::Render(const Tick& tick) {
    VLOG(3) << "drawing screen....";
    const auto size = Window::GetSize();
    auto fb = Renderer::GetFrameBuffer();
    fb->Unbind();
    bloom_.Render();
    pipeline_.Render();
  }
}