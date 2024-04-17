#include "mcc/renderer/render_pass_executor.h"

#include <units.h>
#include "mcc/common.h"
#include "mcc/uv_utils.h"

namespace mcc::render {
  bool RenderPassExecutor::Visit(RenderPass* pass) {
    MCC_ASSERT(pass);
    LOG(INFO) << "executing " << pass->GetName() << "....";
#ifdef MCC_DEBUG
    const auto start_ns = uv_hrtime();
#endif //MCC_DEBUG

    pass->Render();

#ifdef MCC_DEBUG
    using namespace units::time;
    const auto stop_ns = uv_hrtime();
    const auto total_ns = (stop_ns - start_ns);

    auto& stats = pass->stats();
    stats.UpdateTime(total_ns);

    const auto avg_ns = stats.avg_time();
    const auto min_ns = stats.min_time();
    const auto max_ns = stats.max_time();

    LOG(INFO) << pass->GetName() << " finished in " << nanosecond_t(total_ns) << ", avg=" << nanosecond_t(avg_ns) << ", max=" << nanosecond_t(max_ns) << ", min=" << nanosecond_t(min_ns) << ".";
#endif//MCC_DEBUG
    return true;
  }
}