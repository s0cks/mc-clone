#include "mcc/vbo/vbo_id.h"

namespace mcc::vbo {
  rx::observable<VboId> GenerateVboId(const int num) {
    return rx::observable<>::create<VboId>([num](rx::subscriber<VboId> s) {
      VboId ids[num];
      glGenBuffers(num, &ids[0]);
      CHECK_GL(FATAL);

      for(auto idx = 0; idx < num; idx++)
        s.on_next(ids[idx]);
      s.on_completed();
    });
  }
}