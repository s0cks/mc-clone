#include "mcc/render/render_pass_executor.h"

#include <units.h>
#include "mcc/common.h"
#include "mcc/uv/utils.h"

namespace mcc::render {
  bool RenderPassExecutor::Visit(RenderPass* pass) {
    MCC_ASSERT(pass);
    if(pass->ShouldSkip()) {
      DLOG(INFO) << "skipping " << pass->GetName() << ".";
      return true;
    }

    LOG(INFO) << "executing " << pass->GetName() << "....";
#ifdef MCC_DEBUG
    const auto start_ns = uv_hrtime();
#endif //MCC_DEBUG

    pass->Render();
    LOG_IF(WARNING, !pass->VisitChildren(this)) << "failed to visit children.";

#ifdef MCC_DEBUG
    using namespace units::time;
    const auto stop_ns = uv_hrtime();
    const auto total_ns = (stop_ns - start_ns);

    pass->UpdateStats(total_ns, 0, 0);
    const auto& stats = pass->GetStats();
    const auto& duration = stats.duration();
    LOG(INFO) << pass->GetName() << " finished in " << nanosecond_t(total_ns) << ", stats (avg/min/max): " << nanosecond_t(duration.average()) << "," << nanosecond_t(duration.min()) << "," << nanosecond_t(duration.max()) << ".";
#endif//MCC_DEBUG
    return true;
  }
}