#include "mcc/engine/engine_stage.h"
#include "mcc/engine/engine.h"

namespace mcc::engine {
  void Stage::RunStage(const uv_run_mode mode) {
    const auto state = GetState();
    LOG(INFO) << "running " << state << "....";
    const auto start = uv_hrtime();
    Engine::SetState(state);
    uv_run(loop_, mode);
    const auto total_ns = (uv_hrtime() - start);
    DLOG(INFO) << state << " done in " << (total_ns / NSEC_PER_MSEC) << "ms.";
  }
}