#ifndef MCC_PERSPECTIVE_CAMERA_H
#define MCC_PERSPECTIVE_CAMERA_H

#include "mcc/gfx.h"
#include "mcc/common.h"
#include "mcc/input/mouse.h"
#include "mcc/engine/engine.h"
#include "mcc/component/component.h"

namespace mcc::camera {
  class PerspectiveCameraBehavior;
  struct PerspectiveCamera {
    glm::vec3 pos;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 world_up;
    float yaw;
    float pitch;
    float speed;
    float sensitivity;
    float zoom;
    glm::mat4 projection;
    glm::mat4 view;

    glm::mat4 GetProjectionMatrix() const;
    glm::mat4 GetViewMatrix() const;

    friend class PerspectiveCameraBehavior;
    DECLARE_COMPONENT(PerspectiveCamera);
  private:
    static void OnPostInit();
    static void OnInit();
    static void OnPreInit();
  public:
    static void Init();
  };

  static constexpr const glm::vec3 kDefaultCameraPosition = glm::vec3(2.0f, 2.0f, 0.0f);
  class PerspectiveCameraBehavior {
    DEFINE_NON_INSTANTIABLE_TYPE(PerspectiveCameraBehavior);
  private:
    static Entity CreateCameraEntity(const glm::vec3 pos = kDefaultCameraPosition);
    static void SetCameraEntity(const Entity e);
    static void OnMousePosition(const MousePosition& pos);
    
    static void OnTick(const Tick& tick);
    static void OnPreInit();
    static void OnInit();
    static void OnPostInit();
  public:
    static void RegisterComponents();
    static void Init();
    static Entity GetCameraEntity();
    static std::optional<ComponentState<PerspectiveCamera>> GetCameraComponent();
    static Signature GetSignature();
    static bool VisitEntities(std::function<bool(const Entity&)> callback);
  };
}

#endif //MCC_PERSPECTIVE_CAMERA_H