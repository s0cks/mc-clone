#include "mcc/engine/engine_state.h"
#include "mcc/engine/engine.h"
#include "mcc/render/renderer.h"

namespace mcc::engine {
  TickState::TickState(Engine* engine):
    EngineState(engine),
    ticker_(engine->events(), engine->GetLoop()),
    render_ticker_(engine->GetLoop()),
    shutdown_(engine->GetLoop(), this) {
  }

  void TickState::Run() {
    const auto start_ns = uv_hrtime();
    GetEngine()->SetRunning(true);
    uv_run(GetEngine()->GetLoop(), UV_RUN_DEFAULT);
    GetEngine()->SetRunning(false);
    const auto stop_ns = uv_hrtime();
    const auto total_ns = (stop_ns - start_ns);
    AppendDuration(total_ns);
  }

  void TickState::Stop() {
    shutdown_.Call();
  }

  void TickState::OnShutdown() {
    ticker_.Stop();
    render_ticker_.Stop();
    const auto loop = GetEngine()->GetLoop();
    uv_stop(loop);
  }
}