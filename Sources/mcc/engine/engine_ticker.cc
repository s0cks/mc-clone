#include "mcc/engine/engine_ticker.h"

#include "mcc/gfx.h"
#include "mcc/engine/engine.h"

namespace mcc::engine {
  void EngineTicker::OnPreTick() {
    const auto engine = Engine::GetEngine();
    MCC_ASSERT(engine);
    glfwPollEvents();
    CHECK_GL(FATAL);
    engine->Publish<PreTickEvent>();
  }

  void EngineTicker::OnTick(const uv::Tick& tick) {
    const auto engine = Engine::GetEngine();
    MCC_ASSERT(engine);
    engine->Publish<TickEvent>(tick);
  }

  void EngineTicker::OnPostTick(const uv::Tick& tick) {
    const auto engine = Engine::GetEngine();
    MCC_ASSERT(engine);
    engine->Publish<PostTickEvent>(tick);
  }
}