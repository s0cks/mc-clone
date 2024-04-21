#ifndef MCC_FRAMEBUFFER_CONSTANTS_H
#define MCC_FRAMEBUFFER_CONSTANTS_H

#include "mcc/gfx.h"

namespace mcc::framebuffer {
  typedef GLuint FrameBufferObjectId;
  static constexpr const FrameBufferObjectId kInvalidFrameBufferId = -1;
  static constexpr const FrameBufferObjectId kDefaultFrameBufferObjectId = 0;

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