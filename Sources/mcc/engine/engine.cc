#include "mcc/engine/engine.h"

#include "mcc/common.h"
#include "mcc/thread_local.h"
#include "mcc/renderer/renderer.h"

#include "mcc/keyboard/keyboard.h"

namespace mcc::engine {
  void Engine::Run() {
    RunState<InitState>();
    RunState<TickState>();
    RunState<TerminatedState>();
  }

  void Engine::Shutdown() {
    if(current_state_)
      current_state_->Stop();
  }

  static ThreadLocal<Engine> engine_;

  Engine* Engine::GetEngine() {
    if(engine_)
      return engine_.Get();
    const auto engine = new Engine(uv_loop_new());
    engine_.Set(engine);
    return engine;
  }
}