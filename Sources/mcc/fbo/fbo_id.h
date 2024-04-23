#ifndef MCC_FBO_ID_H
#define MCC_FBO_ID_H

#include "mcc/gfx.h"

namespace mcc {
  namespace fbo {
    typedef GLuint FboId;
    static constexpr const FboId kInvalidFrameBufferId = -1;
    static constexpr const FboId kDefaultFrameBufferObjectId = 0;
  }
  using fbo::FboId;
  using fbo::kInvalidFrameBufferId;
  using fbo::kDefaultFrameBufferObjectId;
}

#endif //MCC_FBO_ID_H