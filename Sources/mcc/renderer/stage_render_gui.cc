#include "mcc/renderer/stage.h"
#include "mcc/renderer/renderer.h"
#include "mcc/window.h"
#include "mcc/gui/gui.h"

#include "mcc/bloom.h"
#include "mcc/framebuffer_pipeline.h"

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

    BloomPipeline<> bloom(fb, Dimension(size[0], size[1]), GetShader("blur"));
    bloom.Render();
    
    RenderFrameBufferPipeline pipeline(fb, FrameBufferObject((const BufferObjectId)0), nullptr, kColorAndDepthClearMask);
    pipeline.AddChild(new ApplyPipeline([&]() {
      fb->GetColorBuffer(0)->Bind(0);
      bloom.GetFrameBuffer(0)->GetColorBuffer(0)->Bind(1);
    }));
    pipeline.AddChild(new ApplyShaderPipeline(shader_, [](const ShaderRef& shader) {
      shader->SetInt("tex", 0);
      shader->SetInt("bloomTex", 1);
      shader->SetBool("bloom", true);
      shader->SetBool("hdr", true);
      shader->SetFloat("gamma", 2.2f);
      shader->SetFloat("exposure", 1.0f);
    }));
    pipeline.Render();
  }
}