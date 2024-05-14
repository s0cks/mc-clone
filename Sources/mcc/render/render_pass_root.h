#ifndef MCC_RENDER_PASS_ROOT_H
#define MCC_RENDER_PASS_ROOT_H

#include "mcc/color.h"
#include "mcc/render/render_pass.h"

namespace mcc::render {
  class RenderPassGuis;
  class SkyboxRenderPass;
  class TerrainRenderPass;
  class TextRenderPass;
  class RootRenderPass : public OrderedSequenceRenderPass {
  protected:
    TerrainRenderPass* terrain_;
    RenderPassGuis* guis_;
    SkyboxRenderPass* skybox_;
    TextRenderPass* text_;
    Color clear_;

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

    SkyboxRenderPass* GetSkyboxPass() const {
      return skybox_;
    }
  };
}

#endif //MCC_RENDER_PASS_ROOT_H