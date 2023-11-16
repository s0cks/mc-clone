#include "mcc/renderer/stage.h"
#include "mcc/renderer/renderer.h"
#include "mcc/lighting/ambient_light.h"
#include "mcc/physics/transform.h"

#include "mcc/camera/perspective_camera.h"

namespace mcc::renderer {
  void RenderEntitiesStage::RenderEntity(const Entity e) {
    const auto renderable = Renderable::GetState(e);
    const auto transform = physics::Transform::GetState(e);
    VLOG(1) << "rendering entity " << e << " w/ " << *(*renderable).data();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, (*transform)->position);
    // model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
    const auto& shader = (*renderable)->shader;
    const auto& texture = (*renderable)->texture;
    const auto& material = (*renderable)->material;

    glm::vec3 lightColor = glm::vec3(1.0f, 0.0f, 0.0f);
    glm::vec3 lightPos = glm::vec3(0.0f);
    AmbientLight::Visit([&lightColor,&lightPos](const Entity& l, const ComponentState<AmbientLight>& light) {
      lightColor = light->color;

      const auto lt = physics::Transform::GetState(l);
      LOG_IF(FATAL, !lt) << "no transform found for AmbientLight component of entity " << l;
      lightPos = (*lt)->position;
      return true;
    });

    if(texture.valid()) 
      texture->Bind(0);

    shader->ApplyShader();
    shader->SetMat4("model", model);
    const auto diffuseColor = lightColor * glm::vec3(0.5f);
    const auto ambientColor = diffuseColor * glm::vec3(0.8f);
    shader->SetLight("light", lightPos, ambientColor, diffuseColor, glm::vec3(1.0f));
    shader->SetMaterial("material");
    shader->SetInt("tex0", 0);
    shader->SetVec3("lightColor", lightColor);
    shader->SetUniformBlock("Camera", 0);
    shader->ApplyShader();
    const auto& mesh = (*renderable)->mesh;
    mesh->Render();
    Renderer::IncrementEntityCounter();
    Renderer::IncrementVertexCounter(mesh->vbo().length());
  }

  void RenderEntitiesStage::Render(const Tick& tick) {
    VLOG(3) << "rendering entities....";
    glDisable(GL_CULL_FACE);
    CHECK_GL(FATAL);
    Renderer::VisitEntities([&](const Entity& e) {
      RenderEntity(e);
      return true;
    });
    glEnable(GL_CULL_FACE);
    CHECK_GL(FATAL);
  }
}