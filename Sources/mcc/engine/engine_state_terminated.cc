#include "mcc/engine/engine_state.h"
#include "mcc/engine/engine.h"

namespace mcc::engine {
  void TerminatedState::Run() {
    GetEngine()->Publish<TerminatingEvent>();
    // do stuffs?
    GetEngine()->Publish<TerminatedEvent>();
  }

  void TerminatedState::Stop() {
    // do stuffs?
  }
}