#include "mcc/render/renderer.h"

#include "mcc/shape.h"
#include "mcc/thread_local.h"
#include "mcc/engine/engine.h"
#include "mcc/window/window.h"
#include "mcc/camera/camera_ortho.h"

#include "mcc/ibo/ibo.h"

#include "mcc/render/render_pass.h"
#include "mcc/render/render_pass_2d.h"
#include "mcc/render/render_pass_3d.h"
#include "mcc/render/render_pass_guis.h"
#include "mcc/render/renderer_stats.h"
#include "mcc/render/render_pass_executor.h"

namespace mcc::render {
  static ThreadLocal<Renderer> renderer_;
  static rx::subject<RenderEvent*> events_;

  rx::observable<RenderEvent*> OnRenderEvent() {
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

  class RendererPipelinePass : public OrderedSequenceRenderPass {
  protected:
    const Renderer* renderer_;
    glm::mat4 projection_;

    inline const Renderer* GetRenderer() const {
      return renderer_;
    }

    inline Renderer::Mode GetRenderMode() const {
      return GetRenderer()->GetMode();
    }

    void Render() override {
      const auto window = Window::Get();
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
      glClearColor(0.9f, 0.9f, 0.9f, 1.0f);
      CHECK_GL(FATAL);
    }
  public:
    explicit RendererPipelinePass(Window* window, const Renderer* renderer):
      OrderedSequenceRenderPass(),
      renderer_(renderer) {
      MCC_ASSERT(renderer);
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
    window::OnWindowOpenedEvent()
      .subscribe([renderer,window](WindowOpenedEvent* event) {
        const auto pass = new RendererPipelinePass(window, renderer);
        pass->Append(new RenderPassGuis());
        renderer->GetPass()->Append(pass);
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