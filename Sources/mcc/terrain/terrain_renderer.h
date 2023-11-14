#ifndef MCC_TERRAIN_RENDERER_H
#define MCC_TERRAIN_RENDERER_H

#include "mcc/gfx.h"

namespace mcc::terrain {
  class TerrainRenderer {
  public:
    TerrainRenderer() = default;
    ~TerrainRenderer() = default;
    void Render();
  private:
    static void OnPostInit();
  public:
    static void Init();
  };
}

#endif //MCC_TERRAIN_RENDERER_H