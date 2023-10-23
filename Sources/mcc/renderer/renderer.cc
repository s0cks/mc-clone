#include "mcc/renderer/renderer.h"
#include "mcc/ecs/coordinator.h"
#include "mcc/engine/engine.h"

#include "mcc/camera/perspective_camera.h"

namespace mcc {
  static Tick last_;
  static RelaxedAtomic<uint64_t> frames_;
  static RelaxedAtomic<uint64_t> fps_;

  static Shader lightingShader_;

  void Renderer::RegisterComponents() {
    Components::Register<Renderable>();
  }

  void Renderer::Init() {
    Systems::Register<Renderer>();
    Signature sig;
    sig.set(Components::GetComponentIdForType<Renderable>());
    Systems::SetSignature<Renderer>(sig);

    lightingShader_ = CompileShader("lighting");

    Engine::Register(&OnTick);
  }

  void Renderer::RenderEntity(const glm::mat4 projection, const glm::mat4 view, const Entity e) {
    const auto& renderable = Components::GetComponent<Renderable>(e);
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
    const auto& shader = renderable.shader;
    const auto& texture = renderable.texture;

    lightingShader_.ApplyShader();
    lightingShader_.SetVec3("objColor", glm::vec3(1.0f, 0.5f, 0.31f));
    lightingShader_.SetVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
    lightingShader_.SetMat4("projection", projection);
    lightingShader_.SetMat4("view", view);
    lightingShader_.SetMat4("model", model);

    //texture.Bind0();
    shader.ApplyShader();
    shader.SetMat4("model", model);
    shader.SetMat4("view", view);
    shader.SetMat4("projection", projection);
    shader.SetInt("texture1", 0);
    shader.ApplyShader();
    const auto& mesh = renderable.mesh;
    mesh->Render();
  }

  void Renderer::OnTick(Tick& tick) {
    VLOG(10) << tick;
    frames_ += 1;
    if(tick.dts >= (NSEC_PER_MSEC * 1)) {
      const auto diff = (tick.ts - last_.ts);
      fps_ = ((uint64_t) frames_) * (1.0 * (NSEC_PER_SEC / diff));
      last_ = tick;
      frames_ = 0;
    }

    const auto projection = camera::PerspectiveCameraBehavior::CalculateProjectionMatrix();
    const auto view = camera::PerspectiveCameraBehavior::CalculateViewMatrix();
    Systems::ForEachEntityInSystem<Renderer>([&](const Entity& e) {
      RenderEntity(projection, view, e);
    });
  }

  uint64_t Renderer::GetFrameCount() {
    return (uint64_t) frames_;
  }

  uint64_t Renderer::GetFPS() {
    return (uint64_t) fps_;
  }
}