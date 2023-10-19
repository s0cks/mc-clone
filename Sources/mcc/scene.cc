#include "mcc/scene.h"
#include "mcc/mesh/mesh.h"
#include "mcc/camera/camera.h"

namespace mcc::scene {
#define DEFINE_VISIT_FUNCTION(Name) \
  bool Name##Node::Visit(NodeVisitor* vis) { return vis->Visit##Name(this); }
  FOR_EACH_SCENE_NODE(DEFINE_VISIT_FUNCTION);
#undef DEFINE_VISIT_FUNCTION

  bool NodeRenderer::VisitWindow(WindowNode* node) {
    DLOG(INFO) << "rendering " << node->GetName() << "....";
    return node->VisitChildren(this);
  }

  bool NodeRenderer::VisitModel(ModelNode* node) {
    DLOG(INFO) << "rendering " << node->GetName() << "....";
    const auto shader = node->GetShader();
    const auto mesh = node->GetMesh();
    shader.SetMat4("projection", OrthoCamera::GetCurrentProjectionMatrix());
    shader.ApplyShader();
    mesh->Render();
    return true;
  }
}