#ifndef MCC_BLOOM_H
#define MCC_BLOOM_H

#include "mcc/framebuffer.h"

namespace mcc {
  class Bloom {
  private:
    FrameBuffer* a;
    FrameBuffer* b;

    Bloom();
    ~Bloom() = default;
  };
}

#endif //MCC_BLOOM_H