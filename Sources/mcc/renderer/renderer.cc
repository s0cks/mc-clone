#include "mcc/renderer/renderer.h"

#include "mcc/shape.h"
#include "mcc/thread_local.h"
#include "mcc/engine/engine.h"
#include "mcc/window/window.h"
#include "mcc/vertex/vertex.h"
#include "mcc/camera/camera_ortho.h"

#include "mcc/renderer/render_pass.h"
#include "mcc/renderer/render_pass_2d.h"
#include "mcc/renderer/render_pass_3d.h"
#include "mcc/renderer/renderer_stats.h"
#include "mcc/renderer/render_pass_executor.h"

namespace mcc::render {
  static ThreadLocal<Renderer> renderer_;
  static rx::subject<RenderEvent*> events_;

  rx::observable<RenderEvent*> OnEvent() {
    return events_.get_observable();
  }

  void Renderer::Publish(RenderEvent* event) {
    MCC_ASSERT(event);
    const auto& subscriber = events_.get_subscriber();
    subscriber.on_next(event);
  }

  const uv::TickDurationSeries& Renderer::GetTickDurationSeries() const {
    const auto engine = engine::Engine::GetEngine();
    MCC_ASSERT(engine);
    MCC_ASSERT(engine->IsRunning());
    const auto state = ((engine::TickState*) engine->GetCurrentState());
    const auto& render_ticker = state->render_ticker_;
    return render_ticker.GetTickDurationSeries();
  }

  const uv::TicksPerSecond& Renderer::GetTicksPerSecond() const {
    const auto engine = engine::Engine::GetEngine();
    MCC_ASSERT(engine);
    MCC_ASSERT(engine->IsRunning());
    const auto state = ((engine::TickState*) engine->GetCurrentState());
    const auto& render_ticker = state->render_ticker_;
    return render_ticker.GetTicksPerSecond();
  }

  static inline void
  SetThreadRenderer(Renderer* renderer) {
    MCC_ASSERT(renderer);
    renderer_.Set(renderer);
  }

  static inline Renderer*
  GetThreadRenderer() {
    return renderer_.Get();
  }

  static inline Pipeline*
  CreateRenderQuadPipeline(const glm::mat4& projection, const glm::vec2& pos, const glm::vec2& size, const Color color) {
    const auto window = Window::Get();
    d2::VertexList vertices;
    u32::IndexList indices;
    shape::NewCenteredRect(vertices, indices, pos, size, color);
    const auto mesh = d2::NewMesh(vertices, indices);
    const auto apply_shader = program::ApplyProgramPipeline::New("colored_2d", [projection](const ProgramRef& shader) {
      shader->SetMat4("projection", projection);
    });
    return new d2::RenderMeshPipeline(mesh);
  }

  class RendererPipeline : public Pipeline {
  private:
    const Renderer* renderer_;

    inline const Renderer* GetRenderer() const {
      return renderer_;
    }

    inline Renderer::Mode GetRenderMode() const {
      return GetRenderer()->GetMode();
    }
  public:
    RendererPipeline(const Renderer* renderer):
      Pipeline(),
      renderer_(renderer) {
      MCC_ASSERT(renderer);
      const auto window = Window::Get();
      const auto windowSize = window->GetSize();
      OrthoCamera camera(window);
      const auto& projection = camera.GetProjection();
      // AddChild(CreateRenderQuadPipeline(projection, window->GetCenterCoord(), glm::vec2(256), kGreen));
    }
    ~RendererPipeline() override = default;

    void Render() override {
      const auto window = Window::Get();
      const auto fb_size = window->GetFramebufferSize();
      glViewport(0, 0, fb_size[0], fb_size[1]);
      CHECK_GL(FATAL);
      glfwPollEvents();
      CHECK_GL(FATAL);
      //TODO: remove chunk
      glPolygonMode(GL_FRONT_AND_BACK, GetRenderMode());
      CHECK_GL(FATAL);
      glEnable(GL_DEPTH_TEST);
      CHECK_GL(FATAL);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      CHECK_GL(FATAL);
      glDepthFunc(GL_LEQUAL);
      CHECK_GL(FATAL);
      glEnable(GL_CULL_FACE);
      CHECK_GL(FATAL);
      glFrontFace(GL_CW);
      CHECK_GL(FATAL);
      glCullFace(GL_BACK);
      CHECK_GL(FATAL);
      glClearColor(0.4, 0.3, 0.4, 1.0f);
      CHECK_GL(FATAL);

      RenderChildren();

      window->SwapBuffers();
      //TODO:
      // Renderer::ResetEntityCounter();
      // Renderer::ResetVertexCounter();
    }
  };

  class RendererPipelinePass : public RenderPass {
  protected:
    RendererPipeline pipeline_;

    void Render() override {
      pipeline_.Render();
    }
  public:
    explicit RendererPipelinePass(const Renderer* renderer):
      RenderPass(),
      pipeline_(renderer) {
    }
    ~RendererPipelinePass() override = default;

    const char* GetName() const override {
      return "RendererPipelinePass";
    }
  };

  void Renderer::Init() {
    const auto window = Window::Get();
    const auto engine = engine::Engine::GetEngine();
    const auto renderer = new Renderer(engine->GetLoop());
    SetThreadRenderer(renderer);
    window->OnOpened()
      .subscribe([renderer](WindowOpenedEvent* event) {
        renderer->GetPass()->Append(new RendererPipelinePass(renderer));
      });
  }

  Renderer* Renderer::Get() {
    return GetThreadRenderer();
  }

  void Renderer::Run(const uv_run_mode mode) {
    LOG(INFO) << "running....";
    const auto start_ns = uv::Now();
    render::RenderPassExecutor::Execute(GetPass());
    const auto stop_ns = uv::Now();
    const auto total_ns = (stop_ns - start_ns);

    using namespace units::time;
    DLOG(INFO) << "renderer finished in " << nanosecond_t(total_ns) << ".";
  }
}