#include "mcc/render/renderer.h"

#include "mcc/thread_local.h"
#include "mcc/engine/engine.h"
#include "mcc/render/renderer_stats.h"
#include "mcc/render/render_pass_root.h"
#include "mcc/render/render_pass_executor.h"

namespace mcc::render {
  static ThreadLocal<Renderer> renderer_;
  static rx::subject<RenderEvent*> events_;

  Renderer::Renderer(uv_loop_t* loop, const Mode mode):
    loop_(loop),
    on_run_(),
    mode_(mode),
    pass_(new RootRenderPass()) {
    MCC_ASSERT(loop);
    SetRenderer(on_run_, this);
    InitAsyncHandle(loop, on_run_, &OnRun);
    Publish<RendererInitializedEvent>(this);
  }

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

  void Renderer::Init() {
    const auto engine = engine::Engine::GetEngine();
    const auto renderer = new Renderer(engine->GetLoop());
    SetThreadRenderer(renderer);
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