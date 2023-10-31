#include "mcc/renderer/stage.h"
#include "mcc/ecs/coordinator.h"
#include "mcc/renderer/renderer.h"

#include "mcc/physics/transform.h"

namespace mcc::renderer {
  void RenderEntitiesStage::RenderEntity(const glm::mat4& projection, const glm::mat4& view, const Entity e) {
    const auto& renderable = Components::GetComponent<Renderable>(e);
    const auto& transform = Components::GetComponent<physics::Transform>(e);
    DLOG(INFO) << "rendering entity " << e << " w/ " << renderable;
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, transform.position);
    model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
    const auto& shader = renderable.shader;
    const auto& texture = renderable.texture;

    // texture.Bind0();
    shader.ApplyShader();
    shader.SetMat4("model", model);
    shader.SetMat4("view", view);
    shader.SetMat4("projection", projection);
    shader.SetVec3("color", glm::vec3(0.0f, 0.0f, 1.0f));
    // shader.SetInt("texture1", 0);
    shader.ApplyShader();
    const auto& mesh = renderable.mesh;
    mesh->Render();
    Renderer::IncrementEntityCounter();
  }

  void RenderEntitiesStage::Render(const Tick& tick, const glm::mat4& projection, const glm::mat4& view) {
    DLOG(INFO) << "rendering entities....";
    glEnable(GL_DEPTH_TEST);
    CHECK_GL(FATAL);
    Systems::ForEachEntityInSystem<Renderer>([&](const Entity& e) {
      RenderEntity(projection, view, e);
    });
    glDisable(GL_DEPTH_TEST);
    CHECK_GL(FATAL);
  }
}