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
    const auto total_ns = nanosecond_t(stop_ns - start_ns);
    LOG(INFO) << pass->GetName() << " finished in " << total_ns;
#endif//MCC_DEBUG
    return true;
  }
}