#include "mcc/engine/engine_stage.h"
#include "mcc/engine/engine.h"

namespace mcc::engine {
  void Stage::RunStage(const uv_run_mode mode) {
    const auto state = GetState();
    VLOG(1) << "running " << state << "....";
    const auto start = uv_hrtime();
    Engine::SetState(state);
    uv_run(loop_, mode);
    const auto total_ns = (uv_hrtime() - start);
    VLOG(1) << state << " done in " << (total_ns / NSEC_PER_MSEC) << "ms.";
  }
}