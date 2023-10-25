#include "mcc/renderer/renderer.h"
#include "mcc/ecs/coordinator.h"
#include "mcc/engine/engine.h"

#include "mcc/shape/square.h"
#include "mcc/shape/plane.h"
#include "mcc/camera/perspective_camera.h"

namespace mcc {
  static Tick last_;
  static RelaxedAtomic<uint64_t> frames_;
  static RelaxedAtomic<uint64_t> fps_;

  static Shader lightingShader_;

  static mesh::Mesh* plane_mesh_;
  static Shader plane_shader_;

  static inline mesh::Mesh*
  GeneratePlaneMesh() {
    VertexList plane_vertices;
    Square::GenerateVertices(plane_vertices);
    IndexList plane_indices;
    Square::GenerateIndices(plane_indices);
    return mesh::IndexedMesh::New(plane_vertices, plane_indices);
  }

  static inline Shader
  GeneratePlaneShader() {
    return GetColorizedShader();
  }

  void Renderer::OnPreInit() {
    Components::Register<Renderable>();
  }

  void Renderer::OnInit() {

  }

  void Renderer::OnPostInit() {
    Systems::Register<Renderer>();
    Signature sig;
    sig.set(Components::GetComponentIdForType<Renderable>());
    Systems::SetSignature<Renderer>(sig);
  }

  void Renderer::Init() {
    Engine::OnPreInit(&OnPreInit);
    Engine::OnInit(&OnInit);
    Engine::OnPostInit(&OnPostInit);
    Engine::OnTick(&OnTick);
  }

  void Renderer::RenderEntity(const glm::mat4 projection, const glm::mat4 view, const Entity e) {
    const auto& renderable = Components::GetComponent<Renderable>(e);
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
    const auto& shader = renderable.shader;
    const auto& texture = renderable.texture;

    texture.Bind0();
    shader.ApplyShader();
    shader.SetMat4("model", model);
    shader.SetMat4("view", view);
    shader.SetMat4("projection", projection);
    shader.SetVec3("color", glm::vec3(0.0f, 0.0f, 1.0f));
    // shader.SetInt("texture1", 0);
    shader.ApplyShader();
    const auto& mesh = renderable.mesh;
    mesh->Render();
  }

  void Renderer::RenderPlane(const glm::mat4 projection, const glm::mat4 view) {
    glm::mat4 model = glm::mat4(1.0f);
    plane_shader_.ApplyShader();
    plane_shader_.SetMat4("model", model);
    plane_shader_.SetMat4("projection", projection);
    plane_shader_.SetMat4("view", view);
    plane_shader_.SetVec3("color", glm::vec3(1.0f, 0.0f, 0.0f));
    plane_shader_.ApplyShader();
    plane_mesh_->Render();
  }

  void Renderer::OnTick(Tick& tick) {
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