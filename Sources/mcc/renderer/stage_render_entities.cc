#include "mcc/renderer/stage.h"
#include "mcc/ecs/coordinator.h"
#include "mcc/renderer/renderer.h"
#include "mcc/lighting/ambient_light.h"
#include "mcc/physics/transform.h"

namespace mcc::renderer {
  void RenderEntitiesStage::RenderEntity(const glm::mat4& projection, const glm::mat4& view, const Entity e) {
    const auto renderable = Renderable::GetState(e);
    const auto transform = physics::Transform::GetState(e);

    VLOG(1) << "rendering entity " << e << " w/ " << *(*renderable).data();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, (*transform)->position);
    // model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
    const auto& shader = (*renderable)->shader;
    const auto& texture = (*renderable)->texture;

    float intensity = 0.2f;
    glm::vec3 lightColor = glm::vec3(1.0f);
    glm::vec3 lightPos = glm::vec3(0.0f);
    AmbientLight::Visit([&lightColor,&intensity,&lightPos](const Entity& l, const ComponentState<AmbientLight>& light) {
      lightColor = light->color;
      intensity = light->intensity;

      const auto lt = physics::Transform::GetState(l);
      LOG_IF(FATAL, !lt) << "no transform found for AmbientLight component of entity " << l;
      lightPos = (*lt)->position;
      return true;
    });

    texture.Bind0();
    shader.ApplyShader();
    shader.SetMat4("model", model);
    shader.SetMat4("view", view);
    shader.SetMat4("projection", projection);
    shader.SetVec3("lightPos", lightPos);
    shader.SetVec3("lightColor", lightColor);
    shader.SetFloat("lightIntensity", 0.20f);
    shader.SetInt("tex0", 0);
    shader.ApplyShader();
    const auto& mesh = (*renderable)->mesh;
    mesh->Render();
    Renderer::IncrementEntityCounter();
  }

  void RenderEntitiesStage::Render(const Tick& tick, const glm::mat4& projection, const glm::mat4& view) {
    VLOG(3) << "rendering entities....";
    glEnable(GL_DEPTH_TEST);
    CHECK_GL(FATAL);
    Renderer::VisitEntities([&](const Entity& e) {
      RenderEntity(projection, view, e);
      return true;
    });
    glDisable(GL_DEPTH_TEST);
    CHECK_GL(FATAL);
  }
}