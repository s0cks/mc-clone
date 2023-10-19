#include "mcc/scene.h"
#include "mcc/mesh/mesh.h"
#include "mcc/camera/camera.h"

namespace mcc::scene {
#define DEFINE_VISIT_FUNCTION(Name) \
  bool Name##Node::Visit(NodeVisitor* vis) { return vis->Visit##Name(this); }
  FOR_EACH_SCENE_NODE(DEFINE_VISIT_FUNCTION);
#undef DEFINE_VISIT_FUNCTION

  void ModelNode::Update(NodeUpdater* updater) {
    rotation_.Update(updater->GetLoop());
  }

  bool NodeRenderer::VisitShader(ShaderNode* node) {
    DLOG(INFO) << "rendering " << node->GetName() << "....";
    const auto shader = node->GetShader();
    shader.ApplyShader();
    return true;
  }

  bool NodeRenderer::VisitMesh(MeshNode* node) {
    DLOG(INFO) << "rendering " << node->GetName() << "....";
    const auto mesh = node->GetMesh();
    mesh->Render();
    return true;
  }

  bool NodeRenderer::VisitWindow(WindowNode* node) {
    DLOG(INFO) << "rendering " << node->GetName() << "....";
    return node->VisitChildren(this);
  }

  bool NodeRenderer::VisitModel(ModelNode* node) {
    DLOG(INFO) << "rendering " << node->GetName() << "....";
    if(!node->GetShaderNode()->Visit(this)) {
      DLOG(INFO) << "couldn't render " << node->GetShaderNode()->GetName();
      return false;
    }

    const auto camera = PerspectiveCamera::Get();
    glm::mat4 model = node->GetModelMatrix();
    glm::mat4 view = camera->GetViewMatrix();
    glm::mat4 projection = camera->GetProjectionMatrix();
    const auto shader = node->GetShader();
    shader.SetMat4("model", model);
    shader.SetMat4("view", view);
    shader.SetMat4("projection", projection);

    if(!node->GetMeshNode()->Visit(this)) {
      DLOG(INFO) << "couldn't render " << node->GetMeshNode()->GetName();
      return false;
    }
    return true;
  }

  bool NodeUpdater::VisitWindow(WindowNode* node) {
    DLOG(INFO) << "updating " << node->GetName();
    return node->VisitChildren(this);
  }

  bool NodeUpdater::VisitModel(ModelNode* node) {
    DLOG(INFO) << "updating " << node->GetName();
    node->Update(this);
    return true;
  }

  bool NodeUpdater::VisitShader(ShaderNode* node) {
    return true;
  }

  bool NodeUpdater::VisitMesh(MeshNode* node) {
    return true;
  }
}