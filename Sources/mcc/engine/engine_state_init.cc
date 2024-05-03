#include "mcc/engine/engine_state.h"
#include "mcc/engine/engine.h"

namespace mcc::engine {
  void InitState::Run() {
    GetEngine()->Publish<PreInitEvent>();
    DLOG(INFO) << "initializing engine....";
    // do stuffs?
    GetEngine()->Publish<PostInitEvent>();
  }

  void InitState::Stop() {
    // do stuffs?
  }
}