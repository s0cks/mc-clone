#ifndef MCC_FRAMEBUFFER_FACTORY_H
#define MCC_FRAMEBUFFER_FACTORY_H

#include "mcc/framebuffer/framebuffer_constants.h"

namespace mcc::fbuff {
  class FrameBufferFactory {
  public:
    FrameBufferFactory() = default;
    virtual ~FrameBufferFactory() = default;
    
    virtual FrameBufferTarget GetTarget() const {
      return kFrameBuffer;
    }

    virtual FrameBufferId Create();
  };
}

#endif //MCC_FRAMEBUFFER_FACTORY_H