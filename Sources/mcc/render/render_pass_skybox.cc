#include "mcc/render/render_pass_skybox.h"
#include "mcc/skybox/skybox_mesh.h"

#include "mcc/vao/vao_scope.h"
#include "mcc/vbo/vbo_scope.h"

#include "mcc/camera/camera_perspective.h"

namespace mcc::render {
  SkyboxRenderPass::SkyboxRenderPass():
    RenderPass(),
    skybox_(nullptr),
    program_(Program::New("skybox")) {
  }

  SkyboxRenderPass::~SkyboxRenderPass() {
    delete skybox_;
  }

  bool SkyboxRenderPass::ShouldSkip() const {
    return !HasSkybox();
  }

  void SkyboxRenderPass::Render() {
    MCC_ASSERT(skybox_);
    DLOG(INFO) << "rendering skybox....";
    const auto mesh = GetSkybox()->GetMesh();
    const auto vao = mesh->GetVao();
    const auto vbo = mesh->GetVbo();

    const auto camera = camera::GetPerspectiveCamera();
    MCC_ASSERT(camera);

    const auto model = glm::mat4(1.0f);
    vao::VaoBindScope vao_scope(vao);
    vbo::VboDrawScope draw_scope(vbo);
    program_->Apply();
    program_->SetMat4("projection", camera->GetProjection());
    program_->SetMat4("view", camera->GetView());
    program_->SetMat4("model", model);
    program_->Apply();
    draw_scope.Draw(GL_TRIANGLES);
  }
}