#include "mcc/vao/vao_finalizer.h"

#include <units.h>
#include "mcc/vao/vao.h"
#include "mcc/counter.h"
#include "mcc/uv/utils.h"
#include "mcc/thread_local.h"

namespace mcc::vao {
  static ThreadLocal<VaoFinalizer> finalizer_;

  void VaoFinalizer::Finalize(Vao* vao) {
    DLOG(INFO) << "finalizing " << vao->ToString() << ".....";

    Vao::DeleteVao(vao->GetId());
    delete vao;

    num_finalized_ += 1;
  }

  void VaoFinalizer::FinalizeQueue() {
    DLOG(INFO) << "finalizing all Vaos in queue.....";
    const auto start_ns = uv::Now();
    do {
      const auto next = queue_.Pop();
      if(next == 0)
        break;
      Finalize((Vao*) next);
    } while(true);
    const auto stop_ns = uv::Now();
    const auto total_ns = (stop_ns - start_ns);
#ifdef MCC_DEBUG
    using namespace units::time;
    DLOG(INFO) << "finalized " << num_finalized_ << " Vaos in " << nanosecond_t(total_ns);
#endif //MCC_DEBUG
  }
}