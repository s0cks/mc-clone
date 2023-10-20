#include "mcc/renderer/renderer.h"
#include "mcc/ecs/coordinator.h"
#include "mcc/camera/camera.h"

namespace mcc {
  void Renderer::Update(const float dt) {
    const auto camera = PerspectiveCamera::Get();
    const auto coord = Coordinator::Get();
    for(const auto e : entities_) {
      const auto& renderable = coord->GetComponent<Renderable>(e);
      glm::mat4 model = glm::mat4(1.0f);
      glm::mat4 view = camera->GetViewMatrix();
      glm::mat4 projection = camera->GetProjectionMatrix();
      const auto& shader = renderable.shader;
      shader.ApplyShader();
      shader.SetMat4("model", model);
      shader.SetMat4("view", view);
      shader.SetMat4("projection", projection);
      shader.SetVec3("color", renderable.color);
      shader.ApplyShader();
      const auto& mesh = renderable.mesh;
      mesh->Render();
    }
  }
}