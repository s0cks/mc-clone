#include "mcc/engine/engine_state.h"

#include "mcc/engine/engine.h"
#include "mcc/engine/engine_event.h"

namespace mcc::engine {
  void InitState::Apply() {
    engine()->Publish<PreInitEvent>();
    DLOG(INFO) << "initializing engine....";
    // do stuffs?
    engine()->Publish<PostInitEvent>();
  }
}