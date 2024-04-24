#ifndef MCC_RBO_ID_H
#define MCC_RBO_ID_H

#include "mcc/rx.h"
#include "mcc/gfx.h"

namespace mcc {
  namespace rbo {
    typedef GLuint RboId;
    static constexpr const RboId kInvalidRboId = -1;
    static constexpr const RboId kDefaultRboId = 0;

    static inline bool
    IsValidRboId(const RboId id) {
      return id >= kDefaultRboId;
    }

    static inline bool
    IsDefaultRboId(const RboId id) {
      return id == kDefaultRboId;
    }

    static inline rx::observable<RboId>
    GenerateRboId(const int num = 1) {
      return rx::observable<>::create<RboId>([num](rx::subscriber<RboId> s) {
        RboId ids[num];
        glGenRenderbuffers(num, &ids[0]);
        CHECK_GL(FATAL);

        for(auto idx = 0; idx < num; idx++)
          s.on_next(ids[idx]);
        s.on_completed();
      });
    }
  }
  using rbo::RboId;
  using rbo::kInvalidRboId;
  using rbo::kDefaultRboId;
}

#endif //MCC_RBO_ID_H