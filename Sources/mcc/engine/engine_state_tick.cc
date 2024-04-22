#include "mcc/engine/engine_state.h"

#include "mcc/engine/engine.h"
#include "mcc/engine/engine_event.h"

#include "mcc/renderer/renderer.h"

namespace mcc::engine {
  TickState::TickState(Engine* engine):
    State(engine),
    idle_(),
    prepare_(),
    check_(),
    on_shutdown_(engine->GetLoop(), &OnShutdown, this) { // render_timer_(render::Renderer::Get(), render::Renderer::kDefaultTargetFramesPerSecond, &OnRenderTick) 
    const auto loop = engine->GetLoop();
    SetState(&idle_, this);
    uv_idle_init(loop, &idle_);
    uv_idle_start(&idle_, &OnIdle);

    SetState(&prepare_, this);
    uv_prepare_init(loop, &prepare_);
    uv_prepare_start(&prepare_, &OnPrepare);

    SetState(&check_, this);
    uv_check_init(loop, &check_);
    uv_check_start(&check_, &OnCheck);
  }

  void TickState::OnRenderTick(const render::RenderTimer::Tick& tick) {
    const auto renderer = render::Renderer::Get();
    renderer->Schedule();
  }

  void TickState::OnIdle(uv_idle_t* handle) {
    const auto state = GetState(handle);
    const auto engine = state->engine();
    engine->Publish<PreTickEvent>();
    engine->DoPreTick();
  }

  void TickState::OnPrepare(uv_prepare_t* handle) {
    const auto state = GetState(handle);
    const auto engine = state->engine();
    engine->Publish<TickEvent>(engine->GetCurrentTick());
  }

  void TickState::OnCheck(uv_check_t* handle) {
    const auto state = GetState(handle);
    const auto engine = state->engine();
    engine->Publish<PostTickEvent>(engine->GetCurrentTick());
    engine->DoPostTick();
  }

  void TickState::OnShutdown(TickState* state) {
    const auto loop = state->engine()->GetLoop();
    uv_idle_stop(&state->idle_);
    uv_prepare_stop(&state->prepare_);
    uv_check_stop(&state->check_);
    uv_stop(loop);
  }

  void TickState::Apply() {
    const auto start_ns = uv_hrtime();
    engine()->SetRunning(true);
    uv_run(engine()->GetLoop(), UV_RUN_DEFAULT);
    engine()->SetRunning(false);
    const auto stop_ns = uv_hrtime();
    const auto total_ns = (stop_ns - start_ns);
    duration_.Append(total_ns);
  }

  void TickState::Shutdown() {
    on_shutdown_();
  }
}