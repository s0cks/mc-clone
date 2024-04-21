#include "mcc/framebuffer/framebuffer_factory.h"

namespace mcc::framebuffer {
  FrameBufferObjectId FrameBufferFactory::Create() {
    FrameBufferObjectId id;
    glGenFramebuffers(1, &id);
    CHECK_GL(FATAL);
    glBindFramebuffer(GetTarget(), id);
    CHECK_GL(FATAL);

    //TODO: more?

    glBindFramebuffer(GetTarget(), 0);
    CHECK_GL(FATAL);
    return id;
  }
}