#ifndef MCC_FBO_ID_H
#define MCC_FBO_ID_H

#include "mcc/rx.h"
#include "mcc/gfx.h"

namespace mcc {
  namespace fbo {
    typedef GLuint FboId;
    static constexpr const FboId kInvalidFboId = -1;
    static constexpr const FboId kDefaultFboId = 0;

    static inline bool IsValidFboId(const FboId id) {
      return id >= kDefaultFboId;
    }

    static inline bool IsDefaultFboId(const FboId id) {
      return id == kDefaultFboId;
    }

    static inline rx::observable<FboId>
    GenerateFboId(const int num = 1) {
      return rx::observable<>::create<FboId>([num](rx::subscriber<FboId> s) {
        FboId ids[num];
        glGenFramebuffers(num, &ids[0]);
        CHECK_GL(FATAL);

        for(auto idx = 0; idx < num; idx++)
          s.on_next(ids[idx]);
        s.on_completed();
      });
    }
  }
  using fbo::FboId;
  using fbo::kInvalidFboId;
  using fbo::kDefaultFboId;
}

#endif //MCC_FBO_ID_H