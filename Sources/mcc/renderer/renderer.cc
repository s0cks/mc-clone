#include "mcc/renderer/renderer.h"
#include "mcc/ecs/coordinator.h"
#include "mcc/camera/camera.h"

namespace mcc {
  void Renderer::OnTick(Tick& tick) {
    const auto camera = PerspectiveCamera::Get();
    for(const auto e : entities_) {
      const auto& renderable = Components::GetComponent<Renderable>(e);
      glm::mat4 model = glm::mat4(1.0f);
      glm::mat4 view = camera->GetViewMatrix();
      glm::mat4 projection = camera->GetProjectionMatrix();
      const auto& shader = renderable.shader;
      const auto& texture = renderable.texture;
      texture.Bind0();
      shader.ApplyShader();
      shader.SetMat4("model", model);
      shader.SetMat4("view", view);
      shader.SetMat4("projection", projection);
      shader.SetInt("texture1", 0);
      shader.ApplyShader();
      const auto& mesh = renderable.mesh;
      mesh->Render();
    }
  }
}