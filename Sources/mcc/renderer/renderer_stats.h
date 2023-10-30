#ifndef MCC_RENDERER_STATS_H
#define MCC_RENDERER_STATS_H

#include "mcc/renderer/renderer_state.h"

namespace mcc::renderer {
  struct RendererStats {
    RendererState state;
    struct {
      uint64_t count;
    } entities;
  };
}

#endif //MCC_RENDERER_STATS_H