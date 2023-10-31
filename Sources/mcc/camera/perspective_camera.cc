#include "mcc/camera/perspective_camera.h"
#include "mcc/renderer/renderer.h"
#include "mcc/ecs/coordinator.h"
#include "mcc/camera/camera.h"
#include "mcc/engine/engine.h"

#include "mcc/mouse/mouse.h"
#include "mcc/keyboard/keyboard.h"
#include "mcc/window.h"

namespace mcc::camera {
  static RelaxedAtomic<EntityId> entity_;

  static inline void
  UpdateCamera(PerspectiveCamera& camera) {
    glm::vec3 front;
    front.x = cos(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));
    front.y = sin(glm::radians(camera.pitch));
    front.z = sin(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));
    camera.front = glm::normalize(front);
    camera.right = glm::normalize(glm::cross(camera.front, camera.world_up));
    camera.up = glm::normalize(glm::cross(camera.right, camera.front));
  }

  void PerspectiveCameraBehavior::OnMousePosition(const MousePosition& pos) {
    const auto window = Window::GetHandle();
    if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
      return;

    Systems::ForEachEntityInSystem<PerspectiveCameraBehavior>([&](const Entity& e) {
      auto& camera = Components::GetComponent<PerspectiveCamera>(e);
      camera.yaw += (pos[0] * camera.sensitivity);
      camera.pitch += (pos[1] * camera.sensitivity);
      if(camera.pitch > 89.0f)
        camera.pitch = 89.0f;
      else if(camera.pitch < -89.0f)
        camera.pitch = -89.0f;
      UpdateCamera(camera);
    });
  }

  void PerspectiveCameraBehavior::SetCameraEntity(const Entity e) {
    entity_ = (EntityId) e;
  }

  Entity PerspectiveCameraBehavior::GetCameraEntity() {
    return Entity((EntityId) entity_);
  }

  Entity PerspectiveCameraBehavior::CreateCameraEntity(const glm::vec3 pos) {
    const auto e = Entities::CreateEntity();
    auto camera = PerspectiveCamera {
      .pos = pos,
      .up = glm::vec3(0.0f, 1.0f, 0.0f),
      .world_up = glm::vec3(0.0f, 1.0f, 0.0f),
      .yaw = -90.0f,
      .pitch = 0.0f,
      .sensitivity = 0.1f,
      .zoom = 45.0f,
      .front = glm::vec3(0.0f, 0.0f, -1.0f),
      .speed = 1.5f,
    };
    DLOG(INFO) << "created camera entity: " << e;
    UpdateCamera(camera);
    Coordinator::AddComponent<PerspectiveCamera>(e, camera);
    return e;
  }

  void PerspectiveCameraBehavior::RegisterComponents() {

  }

  void PerspectiveCameraBehavior::OnPreInit() {
    Components::Register<PerspectiveCamera>();
  }

  void PerspectiveCameraBehavior::OnInit() {

  }

  void PerspectiveCameraBehavior::OnPostInit() {
    Systems::Register<PerspectiveCameraBehavior>();
    Signature sig;
    sig.set(Components::GetComponentIdForType<PerspectiveCamera>());
    Systems::SetSignature<PerspectiveCameraBehavior>(sig);

    SetCameraEntity(CreateCameraEntity());
    Mouse::Register(&OnMousePosition);
  }

  void PerspectiveCameraBehavior::Init() {
    Engine::OnPreInit(&OnPreInit);
    Engine::OnInit(&OnInit);
    Engine::OnPostInit(&OnPostInit);
    Engine::OnTick(&OnTick);
  }

  void PerspectiveCameraBehavior::OnTick(const Tick& tick) {
    const auto window = Window::GetHandle();

    if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
      return;

    Systems::ForEachEntityInSystem<PerspectiveCameraBehavior>([&](const Entity& e) {
      auto& camera = Components::GetComponent<PerspectiveCamera>(e);
      const auto velocity = camera.speed * ((NSEC_PER_SEC / tick.dts) * 0.0005f);
      if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.pos += (camera.front * velocity * 0.05f);
      if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.pos -= (camera.front * velocity * 0.05f);
      if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.pos -= (camera.right * velocity * 0.05f);
      if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.pos += (camera.right * velocity * 0.05f);
    });
  }

  glm::mat4 PerspectiveCameraBehavior::CalculateProjectionMatrix() {
    const auto camera = Components::GetComponent<PerspectiveCamera>(GetCameraEntity());
    const auto window_size = Window::GetSize();
    return glm::perspective(glm::radians(camera.zoom), static_cast<float>(window_size[0] / window_size[1]), 0.1f, 100.0f);
  }

  glm::mat4 PerspectiveCameraBehavior::CalculateViewMatrix() {
    const auto camera = Components::GetComponent<PerspectiveCamera>(GetCameraEntity());
    return glm::lookAt(camera.pos, camera.pos + camera.front, camera.up);
  }
}