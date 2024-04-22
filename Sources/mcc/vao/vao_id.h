#ifndef MCC_VAO_ID_H
#define MCC_VAO_ID_H

#include "mcc/rx.h"
#include "mcc/gfx.h"

namespace mcc {
  namespace vao {
    typedef GLuint VaoId;
    static constexpr const VaoId kDefaultVaoId = 0;
    static constexpr const VaoId kInvalidVaoId = -1;

    static inline bool
    IsValidVaoId(const VaoId id) {
      return id != kInvalidVaoId;
    }

    static inline bool
    IsDefaultVaoId(const VaoId id) {
      return id == kDefaultVaoId;
    }

    static inline rx::observable<VaoId>
    GenerateVaoId(const int num = 1) {
      return rx::observable<>::create<VaoId>([num](rx::subscriber<VaoId> s) {
        VaoId ids[num];
        glGenVertexArrays(num, ids);
        CHECK_GL(FATAL);
        for(auto idx = 0; idx < num; idx++)
          s.on_next(ids[idx]);
        s.on_completed();
      });
    }
  }
  using vao::VaoId;
  using vao::kDefaultVaoId;
  using vao::kInvalidVaoId;
}

#endif //MCC_VAO_ID_H