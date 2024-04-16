#include "mcc/engine/engine_state.h"

#include "mcc/engine/engine.h"
#include "mcc/engine/engine_event.h"

namespace mcc::engine {
  void PreInitState::Apply(Engine* engine) {
    engine->Publish<PreInitEvent>();
  }

  void InitState::Apply(Engine* engine) {
    engine->Publish<InitEvent>();
  }

  void PostInitState::Apply(Engine* engine) {
    engine->Publish<PostInitEvent>();
  }

  void TerminatingState::Apply(Engine* engine) {
    engine->Publish<TerminatingEvent>();
  }

  void TerminatedState::Apply(Engine* engine) {
    engine->Publish<TerminatedEvent>();
  }
}