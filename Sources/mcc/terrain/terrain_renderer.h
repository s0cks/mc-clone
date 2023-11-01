#ifndef MCC_TERRAIN_RENDERER_H
#define MCC_TERRAIN_RENDERER_H

#include "mcc/gfx.h"

namespace mcc::terrain {
  class TerrainRenderer {
  private:
    const glm::mat4& projection_;
    const glm::mat4& view_;
  public:
    TerrainRenderer(const glm::mat4& projection,
                    const glm::mat4& view):
                    projection_(projection),
                    view_(view) {
    } 
    ~TerrainRenderer() = default;
    void Render();
  private:
    static void OnPostInit();
  public:
    static void Init();
  };
}

#endif //MCC_TERRAIN_RENDERER_H