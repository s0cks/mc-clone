#include "mcc/camera/perspective_camera.h"

#include "mcc/renderer/renderer.h"
#include "mcc/camera/camera.h"

#include "mcc/mouse/mouse.h"
#include "mcc/keyboard/keyboard.h"

#include "mcc/window/window.h"
#include "mcc/engine/engine.h"

#include "mcc/thread_local.h"

namespace mcc::camera {
  glm::mat4 PerspectiveCamera::GetProjectionMatrix() const {
    const auto window_size = Window::Get()->GetSize();
    return glm::perspective(glm::radians(zoom), static_cast<float>(window_size[0] / window_size[1]), 0.1f, 1000.0f);
  }

  glm::mat4 PerspectiveCamera::GetViewMatrix() const {
    const auto p = glm::vec3(pos);
    return glm::lookAt(p, p + front, up);
  }

  void PerspectiveCamera::ComputeMatrices() {
    const auto window_size = Window::Get()->GetSize();
    const auto p = glm::vec3(pos);
    view = glm::lookAt(p, p + front, up);
    projection = glm::perspective(glm::radians(zoom), static_cast<float>(window_size[0] / window_size[1]), 0.1f, 1000.0f);
  }

  static ThreadLocal<PerspectiveCameraComponent> component_;

  PerspectiveCameraComponent::PerspectiveCameraComponent():
    StatefulComponent<PerspectiveCamera>(),
    on_mouse_init_(),
    on_mouse_deinit_(),
    on_mouse_move_(),
    on_engine_tick_() {
    const auto engine = engine::Engine::GetEngine();
    on_engine_tick_ = engine->OnTickEvent()
      .subscribe([this](engine::TickEvent* event) {
        return OnTick(event);
      });
    on_mouse_init_ = mouse::OnMouseCreatedEvent()
      .subscribe([this](mouse::MouseCreatedEvent* event) {
        return OnMouseCreated(event);
      });
    on_mouse_deinit_ = mouse::OnMouseDestroyedEvent()
      .subscribe([this](mouse::MouseDestroyedEvent* event) {
        return OnMouseDestroyed(event);
      });
  }

  static inline void
  UpdateCamera(component::ComponentState<PerspectiveCamera>* camera) {
    glm::vec3 front;
    front.x = cos(glm::radians((*camera)->yaw)) * cos(glm::radians((*camera)->pitch));
    front.y = sin(glm::radians((*camera)->pitch));
    front.z = sin(glm::radians((*camera)->yaw)) * cos(glm::radians((*camera)->pitch));
    (*camera)->front = glm::normalize(front);
    (*camera)->right = glm::normalize(glm::cross((*camera)->front, (*camera)->world_up));
    (*camera)->up = glm::normalize(glm::cross((*camera)->right, (*camera)->front));
  }

  void PerspectiveCameraComponent::OnTick(engine::TickEvent* event) {
    for(const auto& state : states()) {
      const auto& tick = event->tick();
      const auto& camera = (*state);
      const auto velocity = (camera->speed * 100.0f) * ((1.0f * tick.delta) / NSEC_PER_SEC);
      const auto window = Window::Get();
      if(glfwGetKey(window->handle(), GLFW_KEY_W) == GLFW_PRESS)
        camera->pos += glm::vec4((camera->front * velocity), 0.0f);
      if(glfwGetKey(window->handle(), GLFW_KEY_S) == GLFW_PRESS)
        camera->pos -= glm::vec4((camera->front * velocity), 0.0f);
      if(glfwGetKey(window->handle(), GLFW_KEY_A) == GLFW_PRESS)
        camera->pos -= glm::vec4((camera->right * velocity), 0.0f);
      if(glfwGetKey(window->handle(), GLFW_KEY_D) == GLFW_PRESS)
        camera->pos += glm::vec4((camera->right * velocity), 0.0f);
    };
  }

  void PerspectiveCameraComponent::OnMouseMove(mouse::MouseMoveEvent* event) {
    for(const auto& state : states()) {
      const auto& delta = event->delta();
      (*state)->yaw += (delta[0] * (*state)->sensitivity);
      (*state)->pitch -= (delta[1] * (*state)->sensitivity);
      if((*state)->pitch > 89.0f)
        (*state)->pitch = 89.0f;
      else if((*state)->pitch < -89.0f)
        (*state)->pitch = -89.0f;
      UpdateCamera(state);
      DLOG(INFO) << *(*state);
    }
  }

  void PerspectiveCameraComponent::OnMouseCreated(mouse::MouseCreatedEvent* event) {
    if(on_mouse_move_.is_subscribed())
      on_mouse_move_.unsubscribe();
    on_mouse_move_ = mouse::OnMouseMoveEvent()
      .subscribe([this](mouse::MouseMoveEvent* event) {
        return OnMouseMove(event);
      });
  }

  void PerspectiveCameraComponent::OnMouseDestroyed(mouse::MouseDestroyedEvent* event) {
    if(on_mouse_move_.is_subscribed())
      on_mouse_move_.unsubscribe();
  }

  static inline void
  Set(PerspectiveCameraComponent* component) {
    component_.Set(component);
  }

  void PerspectiveCameraComponent::Init() {
    Set(new PerspectiveCameraComponent());
  }

  PerspectiveCameraComponent* PerspectiveCameraComponent::Get() {
    return component_.Get();
  }

  // void PerspectiveCameraDataUniformBufferObject::Update(const PerspectiveCameraData* data) {
  //   PerspectiveCameraDataUniformBufferObjectScope scope(*this);
  //   glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(PerspectiveCameraData), data);
  // }

  // void PerspectiveCameraDataUniformBufferObject::UpdateView(const glm::mat4& data) {
  //   PerspectiveCameraDataUniformBufferObjectScope scope(*this);
  //   glBufferSubData(GL_UNIFORM_BUFFER, offsetof(PerspectiveCameraData, view), sizeof(glm::mat4), &data[0]);
  // }

  // static RelaxedAtomic<EntityId> entity_;
  // static EntitySet tracked_;
  // static Signature signature_;

  // bool PerspectiveCameraBehavior::VisitEntities(std::function<bool(const Entity&)> callback) {
  //   for(auto& e : tracked_) {
  //     if(!callback(e))
  //       return false;
  //   }
  //   return true;
  // }

  // void PerspectiveCameraBehavior::SetCameraEntity(const Entity e) {
  //   entity_ = (EntityId) e;
  // }

  // Entity PerspectiveCameraBehavior::GetCameraEntity() {
  //   return Entity((EntityId) entity_);
  // }

  // Entity PerspectiveCameraBehavior::CreateCameraEntity(const glm::vec3 pos) {
  //   const auto e = Entities::CreateEntity();
  //   auto camera = e.AddComponent<PerspectiveCamera>(PerspectiveCamera {
  //     {
  //       .pos = glm::vec4(pos, 1.0f),
  //       .view = glm::mat4(1.0f),
  //       .projection = glm::mat4(1.0f),
  //     },
  //     .up = glm::vec3(0.0f, 1.0f, 0.0f),
  //     .world_up = glm::vec3(0.0f, 1.0f, 0.0f),
  //     .yaw = -90.0f,
  //     .pitch = 0.0f,
  //     .sensitivity = 0.1f,
  //     .zoom = 45.0f,
  //     .front = glm::vec3(0.0f, 0.0f, -1.0f),
  //     .speed = 0.5f,
  //   });
  //   DLOG(INFO) << "created camera entity: " << e;
  //   UpdateCamera(camera);
  //   return e;
  // }

  // void PerspectiveCameraBehavior::RegisterComponents() {

  // }

  // void PerspectiveCameraBehavior::OnInit() {
  //   Entity::OnSignatureChanged()
  //     .subscribe([](EntitySignatureChangedEvent* e) {
  //       const auto& esig = e->signature;
  //       const auto& eid = e->id;
  //       if((esig & signature_) == signature_) {
  //         tracked_.insert(eid);
  //       } else {
  //         tracked_.erase(eid);
  //       }
  //     });
  //   Entity::OnDestroyed()
  //     .subscribe([](EntityDestroyedEvent* e) {
  //       tracked_.erase(e->id);
  //     });
  //   GetMouse()->OnMouseMove()
  //     .subscribe(&OnMouseMove);
  // }

  // void PerspectiveCameraBehavior::OnPostInit() {
  //   DLOG(INFO) << "post-init.";
  //   signature_.set(PerspectiveCamera::GetComponentId());
  //   SetCameraEntity(CreateCameraEntity());
  // }

  // void PerspectiveCameraBehavior::Init() {
  //   const auto engine = engine::Engine::GetEngine();
  //   engine->OnInitEvent().subscribe([](engine::InitEvent* e) {
  //     return PerspectiveCameraBehavior::OnInit();
  //   });
  //   engine->OnPostInitEvent().subscribe([](engine::PostInitEvent* e) {
  //     return PerspectiveCameraBehavior::OnPostInit();
  //   });
  //   engine->OnTickEvent().subscribe([](engine::TickEvent* e) {
  //     const auto& tick = e->tick();
  //     return PerspectiveCameraBehavior::OnTick(tick);
  //   });
  //   PerspectiveCamera::Init();
  // }

  // void PerspectiveCameraBehavior::OnTick(const Tick& tick) {
  //   DLOG(INFO) << "tick.";
  //   const auto window = Window::Get()->handle();

  //   if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
  //     return;

  //   VisitEntities([&](const Entity& e) {
  //     const auto state = PerspectiveCamera::GetState(e);
  //     if(!state) {
  //       LOG(ERROR) << "no state for " << e;
  //       return false;
  //     }

  //     auto camera = state.value();
  //     const auto velocity = (camera->speed * 100.0f) * ((1.0f * tick.dts) / NSEC_PER_SEC);
  //     if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
  //       camera->pos += glm::vec4((camera->front * velocity), 0.0f);
  //     if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
  //       camera->pos -= glm::vec4((camera->front * velocity), 0.0f);
  //     if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
  //       camera->pos -= glm::vec4((camera->right * velocity), 0.0f);
  //     if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
  //       camera->pos += glm::vec4((camera->right * velocity), 0.0f);
  //     return true;
  //   });
  // }

  // std::optional<ComponentState<PerspectiveCamera>> PerspectiveCameraBehavior::GetCameraComponent() {
  //   return PerspectiveCamera::GetState(GetCameraEntity());
  // }

  // Signature PerspectiveCameraBehavior::GetSignature() {
  //   return signature_;
  // }

  // static ThreadLocal<PerspectiveCameraComponent> component_;

  // static inline void
  // Set(PerspectiveCameraComponent* component) {
  //   component_ = component;
  // }
}