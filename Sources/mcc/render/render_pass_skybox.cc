#include "mcc/render/render_pass_skybox.h"

namespace mcc::render {
  SkyboxRenderPass::SkyboxRenderPass():
    RenderPass() {
  }

  SkyboxRenderPass::~SkyboxRenderPass() {
    
  }

  void SkyboxRenderPass::Render() {
    DLOG(INFO) << "rendering skybox....";
  }
}