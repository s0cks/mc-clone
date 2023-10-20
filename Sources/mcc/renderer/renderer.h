#ifndef MCC_RENDERER_H
#define MCC_RENDERER_H

#include "mcc/common.h"
#include "mcc/ecs/system.h"
#include "mcc/renderer/renderable.h"

namespace mcc {
  class Renderer : public TickSystem {
  public:
    Renderer() = default;
    ~Renderer() override = default;
    void OnTick(Tick& tick) override;
  };
}

#endif //MCC_RENDERER_H