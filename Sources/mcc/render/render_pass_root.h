#ifndef MCC_RENDER_PASS_ROOT_H
#define MCC_RENDER_PASS_ROOT_H

#include "mcc/render/render_pass.h"

namespace mcc::render {
  class RenderPassGuis;
  class TerrainRenderPass;
  class RootRenderPass : public OrderedSequenceRenderPass {
  protected:
    TerrainRenderPass* terrain_;
    RenderPassGuis* guis_;

    void Render() override;
  public:
    RootRenderPass();
    ~RootRenderPass() override;

    TerrainRenderPass* GetTerrainPass() const {
      return terrain_;
    }

    RenderPassGuis* GetGuisPass() const {
      return guis_;
    }
  };
}

#endif //MCC_RENDER_PASS_ROOT_H