#ifndef MCC_GFX_TARGET_H
#define MCC_GFX_TARGET_H

#include "mcc/gfx.h"

namespace mcc::gfx {
  enum Target : GLenum {
    kArrayBufferTarget = GL_ARRAY_BUFFER,
    kElementArrayBufferTarget = GL_ELEMENT_ARRAY_BUFFER,

    kVboTarget = kArrayBufferTarget,
    kIboTarget = kElementArrayBufferTarget,
  };
}

#endif //MCC_GFX_TARGET_H