#include "mcc/render/render_pass_skybox.h"
#include "mcc/skybox/skybox_mesh.h"

#include "mcc/vao/vao_scope.h"
#include "mcc/vbo/vbo_scope.h"

#include "mcc/camera/camera_perspective.h"

#include "mcc/cull_face_scope.h"

namespace mcc::render {
  SkyboxRenderPass::SkyboxRenderPass():
    RenderPass(),
    program_(Program::New("skybox")) {
  }

  SkyboxRenderPass::~SkyboxRenderPass() {
  }

  bool SkyboxRenderPass::ShouldSkip() const {
    return skybox::GetCurrentSkybox() == nullptr;
  }

  void SkyboxRenderPass::Render() {
    const auto skybox = skybox::GetCurrentSkybox();
    MCC_ASSERT(skybox);
    DLOG(INFO) << "rendering skybox....";
    const auto texture = skybox->GetTexture(); 
    const auto mesh = skybox->GetMesh();
    const auto vao = mesh->GetVao();
    const auto vbo = mesh->GetVbo();

    const auto camera = camera::GetPerspectiveCamera();
    MCC_ASSERT(camera);

    texture->Bind0();

    glDepthMask(GL_FALSE);
    CHECK_GL(FATAL);
    const auto model = glm::mat4(1.0f);
    vao::VaoBindScope vao_scope(vao);
    vbo::VboDrawScope draw_scope(vbo);
    program_->Apply();
    program_->SetMat4("projection", camera->GetProjection());
    program_->SetMat4("view", camera->GetView());
    program_->SetMat4("model", model);
    program_->SetInt("tex", 0);
    draw_scope.Draw(GL_TRIANGLES);
    glDepthMask(GL_TRUE);
    CHECK_GL(FATAL);
  }
}