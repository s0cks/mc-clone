#include "mcc/engine/engine_ticker.h"

#include "mcc/engine/engine.h"

namespace mcc::engine {
  void EngineTicker::OnIdle() {
    const auto engine = Engine::GetEngine();
    MCC_ASSERT(engine);
    engine->Publish<PreTickEvent>();
  }

  void EngineTicker::OnPrepare() {
    const auto engine = Engine::GetEngine();
    last_ = current_;
    current_ = uv::Now();
    const auto tick = Tick((uint64_t) count_, current_, last_);
    count_.Increment(1, current_);
    engine->Publish<TickEvent>(tick);
  }

  void EngineTicker::OnCheck() {
    const auto engine = Engine::GetEngine();
    MCC_ASSERT(engine);
    const auto tick = Tick(count_ - 1, current_, last_);
    duration_.Append(tick.delta);
    engine->Publish<PostTickEvent>(tick);
  }
}