#ifndef MCC_RENDER_PASS_TERRAIN_H
#define MCC_RENDER_PASS_TERRAIN_H

#include "mcc/render/render_pass.h"

namespace mcc::render {
  class TerrainRenderPass : public RenderPass {
  public:
    TerrainRenderPass() = default;
    ~TerrainRenderPass() override = default;

    bool ShouldSkip() const override;
    void Render() override;
    DECLARE_RENDER_PASS(Terrain);
  };
}

#endif //MCC_RENDER_PASS_TERRAIN_H