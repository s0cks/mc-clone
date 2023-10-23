#ifndef MCC_RENDERER_H
#define MCC_RENDERER_H

#include "mcc/common.h"
#include "mcc/ecs/system.h"
#include "mcc/renderer/renderable.h"

namespace mcc {
  class Renderer {
    DEFINE_NON_INSTANTIABLE_TYPE(Renderer);
  private:
    static void OnTick(Tick& tick);
    static void RenderEntity(const glm::mat4 projection, const glm::mat4 view, const Entity e);
  public:
    static void Init();
    static void RegisterComponents();
    static uint64_t GetFrameCount();
    static uint64_t GetFPS();
  };
}

#endif //MCC_RENDERER_H