#include "mcc/engine/engine_state.h"

#include "mcc/engine/engine.h"
#include "mcc/engine/engine_event.h"

#include "mcc/renderer/renderer.h"

namespace mcc::engine {
  TickState::TickState(Engine* engine):
    State(engine),
    ticker_(engine->events(), engine->GetLoop()),
    render_ticker_(engine->GetLoop()),
    shutdown_(engine->GetLoop(), this) {
  }

  void TickState::Stop() {
    ticker_.Stop();
    render_ticker_.Stop();
    const auto loop = engine()->GetLoop();
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
    shutdown_.Call();
  }

  void TickState::OnShutdown() {
    Stop();
  }
}