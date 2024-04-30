#include "mcc/ubo/ubo_id.h"

namespace mcc::ubo {
  rx::observable<UboId> GenerateUboIds(const int num) {
    return rx::observable<>::create<UboId>([num](rx::subscriber<UboId> s) {
      UboId ids[num];
      glGenBuffers(num, &ids[0]);
      CHECK_GL(FATAL);

      for(auto idx = 0; idx < num; idx++)
        s.on_next(ids[idx]);
      s.on_completed();
    });
  }
}