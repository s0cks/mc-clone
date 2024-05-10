#include "mcc/ibo/ibo_id.h"

namespace mcc::ibo {
  void GenerateIboIds(IboId* ids, const int num_ids) {
    MCC_ASSERT(ids == nullptr);
    MCC_ASSERT(num_ids >= 1);
    glGenBuffers(num_ids, &ids[0]);
    CHECK_GL(FATAL);
  }

  rx::observable<IboId> GenerateIboIdsAsync(const int num_ids) {
    if(num_ids <= 0)
      return rx::observable<>::empty<IboId>();
    return rx::observable<>::create<IboId>([num_ids](rx::subscriber<IboId> s) {
      IboId ids[num_ids];
      GenerateIboIds(&ids[0], num_ids);
      for(auto idx = 0; idx < num_ids; idx++)
        s.on_next(ids[idx]);
      s.on_completed();
    });
  }
}