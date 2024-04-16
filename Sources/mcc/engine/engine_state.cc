#include "mcc/engine/engine_state.h"

#include "mcc/engine/engine.h"
#include "mcc/engine/engine_event.h"

namespace mcc::engine {
  void PreInitState::Apply() {
    engine()->Publish<PreInitEvent>();
  }

  void InitState::Apply() {
    engine()->Publish<InitEvent>();
  }

  void PostInitState::Apply() {
    engine()->Publish<PostInitEvent>();
  }

  void TerminatingState::Apply() {
    engine()->Publish<TerminatingEvent>();
  }

  void TerminatedState::Apply() {
    engine()->Publish<TerminatedEvent>();
  }
}