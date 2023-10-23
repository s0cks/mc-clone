#include "mcc/camera/perspective_camera.h"
#include "mcc/renderer/renderer.h"
#include "mcc/ecs/coordinator.h"
#include "mcc/camera/camera.h"
#include "mcc/engine/engine.h"

#include "mcc/mouse/mouse.h"
#include "mcc/keyboard/keyboard.h"
#include "mcc/window.h"

namespace mcc::camera {
  static RelaxedAtomic<Entity> entity_;

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
    entity_ = e;
  }

  Entity PerspectiveCameraBehavior::GetCameraEntity() {
    return (Entity) entity_;
  }

  Entity PerspectiveCameraBehavior::CreateCameraEntity() {
    const auto e = Entities::CreateEntity();
    auto camera = PerspectiveCamera {
      .pos = glm::vec3(0.0f, 0.0f, 0.0f),
      .up = glm::vec3(0.0f, 1.0f, 0.0f),
      .world_up = glm::vec3(0.0f, 1.0f, 0.0f),
      .yaw = -90.0f,
      .pitch = 0.0f,
      .sensitivity = 0.1f,
      .zoom = 45.0f,
      .front = glm::vec3(0.0f, 0.0f, -1.0f),
      .speed = 2.5f,
    };
    DLOG(INFO) << "created camera entity: " << e;
    UpdateCamera(camera);
    Coordinator::AddComponent<PerspectiveCamera>(e, camera);
    return e;
  }

  void PerspectiveCameraBehavior::RegisterComponents() {
    Components::Register<PerspectiveCamera>();
  }

  void PerspectiveCameraBehavior::Init() {
    Systems::Register<PerspectiveCameraBehavior>();
    Signature sig;
    sig.set(Components::GetComponentIdForType<PerspectiveCamera>());
    Systems::SetSignature<PerspectiveCameraBehavior>(sig);

    SetCameraEntity(CreateCameraEntity());
    Engine::Register(&OnTick);
    Mouse::Register(&OnMousePosition);
  }

  void PerspectiveCameraBehavior::OnTick(Tick& tick) {
    const auto window = Window::GetWindow()->handle();
    Systems::ForEachEntityInSystem<PerspectiveCameraBehavior>([&](const Entity& e) {
      auto& camera = Components::GetComponent<PerspectiveCamera>(e);
      const auto velocity = camera.speed * ((NSEC_PER_SEC / tick.dts) * 0.0025f);
      DLOG(INFO) << "velocity: " << velocity;
      if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.pos += (camera.front * velocity);
      if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.pos -= (camera.front * velocity);
      if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.pos -= (camera.right * velocity);
      if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.pos += (camera.right * velocity);
    });
  }

  glm::mat4 PerspectiveCameraBehavior::CalculateProjectionMatrix() {
    const auto camera = Components::GetComponent<PerspectiveCamera>(GetCameraEntity());
    const auto window_size = Window::GetWindow()->GetSize();
    return glm::perspective(glm::radians(camera.zoom), static_cast<float>(window_size[0] / window_size[1]), 0.1f, 100.0f);
  }

  glm::mat4 PerspectiveCameraBehavior::CalculateViewMatrix() {
    const auto camera = Components::GetComponent<PerspectiveCamera>(GetCameraEntity());
    return glm::lookAt(camera.pos, camera.pos + camera.front, camera.up);
  }
}