#ifndef MCC_FRAMEBUFFER_CONSTANTS_H
#define MCC_FRAMEBUFFER_CONSTANTS_H

#include "mcc/gfx.h"

namespace mcc {
  enum FrameBufferTarget {
    kFrameBuffer = GL_FRAMEBUFFER,

    kDefaultFrameBufferTarget = kFrameBuffer,
  };

  enum FrameBufferTextureAttachment : GLenum {
    kColorAttachment0 = GL_COLOR_ATTACHMENT0,
  };

  enum FrameBufferDepthAttachment : GLenum {
    kDepthAttachment = GL_DEPTH_ATTACHMENT,
    kDepthStencilAttachment = GL_DEPTH_STENCIL_ATTACHMENT,
  };
}

#endif //MCC_FRAMEBUFFER_CONSTANTS_H