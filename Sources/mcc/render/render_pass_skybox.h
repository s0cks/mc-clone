#ifndef MCC_RENDER_PASS_SKYBOX_H
#define MCC_RENDER_PASS_SKYBOX_H

#include "mcc/skybox/skybox.h"
#include "mcc/program/program.h"
#include "mcc/render/render_pass.h"

namespace mcc::render {
  class SkyboxRenderPass : public RenderPass {
  protected:
    Skybox* skybox_;
    ProgramRef program_;

    Order GetOrder() const override {
      return kFirst;
    }

    bool ShouldSkip() const override;

    inline Skybox* GetSkybox() const {
      return skybox_;
    }

    inline bool HasSkybox() const {
      return GetSkybox() != nullptr;
    }
  public:
    SkyboxRenderPass();
    ~SkyboxRenderPass() override;
    void Render() override;
    DECLARE_RENDER_PASS(Skybox);
  };
}

#endif //MCC_RENDER_PASS_SKYBOX_H