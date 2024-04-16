#ifndef MCC_PERSPECTIVE_CAMERA_H
#define MCC_PERSPECTIVE_CAMERA_H

#include "mcc/gfx.h"
#include "mcc/uniform_buffer.h"
#include "mcc/common.h"
#include "mcc/mouse/mouse.h"
#include "mcc/engine/engine.h"
#include "mcc/component/component.h"
#include "mcc/component/component_state_table.h"

namespace mcc::camera {
  class PerspectiveCameraBehavior;

  struct PerspectiveCameraData { //TODO: temporary
    glm::vec4 pos;
    glm::mat4 projection;
    glm::mat4 view;
  };

  struct PerspectiveCamera : public PerspectiveCameraData {
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 world_up;
    float yaw;
    float pitch;
    float speed;
    float sensitivity;
    float zoom;

    glm::mat4 GetProjectionMatrix() const;
    glm::mat4 GetViewMatrix() const;
    void ComputeMatrices();

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
    static void OnMouseMove(mouse::MouseMoveEvent* event);
    
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

  class PerspectiveCameraDataUniformBufferObject : public UniformBufferObjectTemplate<PerspectiveCameraData> {
  public:
    explicit PerspectiveCameraDataUniformBufferObject(const BufferObjectId id):
      UniformBufferObjectTemplate(id) {
    }
    PerspectiveCameraDataUniformBufferObject():
      UniformBufferObjectTemplate((const uint64_t) 1) {
      glBindBufferRange(GL_UNIFORM_BUFFER, 0, id_, 0, sizeof(PerspectiveCameraData));
      CHECK_GL(FATAL);
    }
    ~PerspectiveCameraDataUniformBufferObject() override = default;

    void Update(const PerspectiveCameraData* data);
    void UpdateView(const glm::mat4& view);
  };
  DEFINE_RESOURCE_SCOPE(PerspectiveCameraDataUniformBufferObject);
}

#endif //MCC_PERSPECTIVE_CAMERA_H