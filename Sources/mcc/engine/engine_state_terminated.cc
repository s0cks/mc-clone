#include "mcc/engine/engine_state.h"

#include "mcc/engine/engine.h"
#include "mcc/engine/engine_event.h"

namespace mcc::engine {
  void TerminatedState::Apply() {
    engine()->Publish<TerminatingEvent>();
    engine()->Publish<TerminatedEvent>();
  }
}