#ifndef MCC_PERSPECTIVE_CAMERA_H
#define MCC_PERSPECTIVE_CAMERA_H

#include "mcc/gfx.h"
#include "mcc/common.h"
#include "mcc/ecs/entity.h"
#include "mcc/mouse/mouse.h"
#include "mcc/engine/engine.h"

namespace mcc::camera {
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
  };

  class PerspectiveCameraBehavior {
    DEFINE_NON_INSTANTIABLE_TYPE(PerspectiveCameraBehavior);
  private:
    static Entity CreateCameraEntity();
    static void SetCameraEntity(const Entity e);
    static void OnMousePosition(const MousePosition& pos);
    
    static void OnTick(Tick& tick);
    static void OnPreInit();
    static void OnInit();
    static void OnPostInit();
  public:
    static void RegisterComponents();
    static void Init();
    static Entity GetCameraEntity();
    static glm::mat4 CalculateProjectionMatrix();
    static glm::mat4 CalculateViewMatrix();
  };
}

#endif //MCC_PERSPECTIVE_CAMERA_H