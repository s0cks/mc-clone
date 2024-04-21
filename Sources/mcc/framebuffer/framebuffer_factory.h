#ifndef MCC_FRAMEBUFFER_FACTORY_H
#define MCC_FRAMEBUFFER_FACTORY_H

#include "mcc/framebuffer/framebuffer_constants.h"

namespace mcc::framebuffer {
  class FrameBufferFactory {
  public:
    FrameBufferFactory() = default;
    virtual ~FrameBufferFactory() = default;
    
    virtual FrameBufferTarget GetTarget() const {
      return kFrameBuffer;
    }

    virtual FrameBufferObjectId Create();
  };
}

#endif //MCC_FRAMEBUFFER_FACTORY_H