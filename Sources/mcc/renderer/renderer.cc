#include "mcc/renderer/renderer.h"
#include "mcc/ecs/coordinator.h"
#include "mcc/camera/camera.h"
#include "mcc/engine/engine.h"

namespace mcc {
  static Tick last_;
  static RelaxedAtomic<uint64_t> frames_;
  static RelaxedAtomic<uint64_t> fps_;

  void Renderer::Init() {
    Engine::Register(&OnTick);
    Systems::Register<Renderer>();
    Components::Register<Renderable>();

    Signature sig;
    sig.set(Components::GetComponentIdForType<Renderable>());
    Systems::SetSignature<Renderer>(sig);
  }

  void Renderer::RenderEntity(const Entity e) {
    const auto camera = PerspectiveCamera::Get();
    const auto& renderable = Components::GetComponent<Renderable>(e);
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
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

  void Renderer::OnTick(Tick& tick) {
    DLOG(INFO) << tick;
    frames_ += 1;
    if(tick.dts >= (NSEC_PER_SEC * 0.01)) {
      const auto diff = (tick.ts - last_.ts);
      fps_ = (1.0 / (diff / NSEC_PER_SEC)) * (uint64_t) frames_;
      last_ = tick;
      frames_ = 0;
    }
    Systems::ForEachEntityInSystem<Renderer>(&RenderEntity);
  }

  uint64_t Renderer::GetFrameCount() {
    return (uint64_t) frames_;
  }

  uint64_t Renderer::GetFPS() {
    return (uint64_t) fps_;
  }
}