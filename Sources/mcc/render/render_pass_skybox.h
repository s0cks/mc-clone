#ifndef MCC_RENDER_PASS_SKYBOX_H
#define MCC_RENDER_PASS_SKYBOX_H

#include "mcc/render/render_pass.h"

namespace mcc::render {
  class SkyboxRenderPass : public RenderPass {
  protected:
    Order GetOrder() const override {
      return kFirst;
    }
  public:
    SkyboxRenderPass();
    ~SkyboxRenderPass() override;
    void Render() override;
    DECLARE_RENDER_PASS(Skybox);
  };
}

#endif //MCC_RENDER_PASS_SKYBOX_H