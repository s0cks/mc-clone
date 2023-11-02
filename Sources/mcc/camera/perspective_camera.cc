#include "mcc/camera/perspective_camera.h"
#include "mcc/renderer/renderer.h"
#include "mcc/ecs/coordinator.h"
#include "mcc/camera/camera.h"
#include "mcc/engine/engine.h"

#include "mcc/mouse/mouse.h"
#include "mcc/keyboard/keyboard.h"
#include "mcc/window.h"

namespace mcc::camera {
  DEFINE_COMPONENT(PerspectiveCamera);

  void PerspectiveCamera::OnPreInit() {
    RegisterComponent();
  }

  void PerspectiveCamera::OnInit() {

  }

  void PerspectiveCamera::OnPostInit() {

  }

  void PerspectiveCamera::Init() {
    Engine::OnPreInit(&OnPreInit);
    Engine::OnInit(&OnInit);
    Engine::OnPostInit(&OnPostInit);
    Entity::OnDestroyed().subscribe(&OnEntityDestroyed);
  }

  glm::mat4 PerspectiveCamera::GetProjectionMatrix() const {
    const auto window_size = Window::GetSize();
    return glm::perspective(glm::radians(zoom), static_cast<float>(window_size[0] / window_size[1]), 0.1f, 1000.0f);
  }

  glm::mat4 PerspectiveCamera::GetViewMatrix() const {
    return glm::lookAt(pos, pos + front, up);
  }

  static RelaxedAtomic<EntityId> entity_;
  static EntitySet tracked_;
  static Signature signature_;

  static inline void
  UpdateCamera(ComponentState<PerspectiveCamera> camera) {
    glm::vec3 front;
    front.x = cos(glm::radians(camera->yaw)) * cos(glm::radians(camera->pitch));
    front.y = sin(glm::radians(camera->pitch));
    front.z = sin(glm::radians(camera->yaw)) * cos(glm::radians(camera->pitch));
    camera->front = glm::normalize(front);
    camera->right = glm::normalize(glm::cross(camera->front, camera->world_up));
    camera->up = glm::normalize(glm::cross(camera->right, camera->front));
  }

  bool PerspectiveCameraBehavior::VisitEntities(std::function<bool(const Entity&)> callback) {
    for(auto& e : tracked_) {
      if(!callback(e))
        return false;
    }
    return true;
  }

  void PerspectiveCameraBehavior::OnMousePosition(const MousePosition& pos) {
    const auto window = Window::GetHandle();
    if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
      return;
    VisitEntities([&](const Entity& e) {
      auto state = PerspectiveCamera::GetState(e);
      if(!state) {
        LOG(WARNING) << "no state for " << e;
        return false;
      }

      auto camera = state.value();
      camera->yaw += (pos[0] * camera->sensitivity);
      camera->pitch += (pos[1] * camera->sensitivity);
      if(camera->pitch > 89.0f)
        camera->pitch = 89.0f;
      else if(camera->pitch < -89.0f)
        camera->pitch = -89.0f;
      UpdateCamera(camera);
      return true;
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
    auto camera = Coordinator::AddComponent(e, PerspectiveCamera {
      .pos = pos,
      .up = glm::vec3(0.0f, 1.0f, 0.0f),
      .world_up = glm::vec3(0.0f, 1.0f, 0.0f),
      .yaw = -90.0f,
      .pitch = 0.0f,
      .sensitivity = 0.1f,
      .zoom = 45.0f,
      .front = glm::vec3(0.0f, 0.0f, -1.0f),
      .speed = 1.5f,
    });
    DLOG(INFO) << "created camera entity: " << e;
    UpdateCamera(camera);
    return e;
  }

  void PerspectiveCameraBehavior::RegisterComponents() {

  }

  void PerspectiveCameraBehavior::OnPreInit() {

  }

  void PerspectiveCameraBehavior::OnInit() {
    Entity::OnSignatureChanged()
      .subscribe([](EntitySignatureChangedEvent* e) {
        const auto& esig = e->signature;
        const auto& eid = e->id;
        if((esig & signature_) == signature_) {
          tracked_.insert(eid);
        } else {
          tracked_.erase(eid);
        }
      });
    Entity::OnDestroyed()
      .subscribe([](EntityDestroyedEvent* e) {
        tracked_.erase(e->id);
      });
  }

  void PerspectiveCameraBehavior::OnPostInit() {
    signature_.set(PerspectiveCamera::GetComponentId());
    SetCameraEntity(CreateCameraEntity());
    Mouse::Register(&OnMousePosition);
  }

  void PerspectiveCameraBehavior::Init() {
    Engine::OnPreInit(&OnPreInit);
    Engine::OnInit(&OnInit);
    Engine::OnPostInit(&OnPostInit);
    Engine::OnTick(&OnTick);
    PerspectiveCamera::Init();
  }

  void PerspectiveCameraBehavior::OnTick(const Tick& tick) {
    const auto window = Window::GetHandle();

    if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
      return;

    VisitEntities([&](const Entity& e) {
      const auto state = PerspectiveCamera::GetState(e);
      if(!state) {
        LOG(ERROR) << "no state for " << e;
        return false;
      }

      auto camera = state.value();
      const auto velocity = camera->speed * ((NSEC_PER_SEC / tick.dts) * 0.0005f);
      if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera->pos += (camera->front * velocity * 0.05f);
      if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera->pos -= (camera->front * velocity * 0.05f);
      if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera->pos -= (camera->right * velocity * 0.05f);
      if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera->pos += (camera->right * velocity * 0.05f);
      return true;
    });
  }

  std::optional<ComponentState<PerspectiveCamera>> PerspectiveCameraBehavior::GetCameraComponent() {
    return PerspectiveCamera::GetState(GetCameraEntity());
  }

  Signature PerspectiveCameraBehavior::GetSignature() {
    return signature_;
  }
}