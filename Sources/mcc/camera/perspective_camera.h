#ifndef MCC_PERSPECTIVE_CAMERA_H
#define MCC_PERSPECTIVE_CAMERA_H

#include "mcc/gfx.h"
#include "mcc/mouse/mouse.h"
#include "mcc/uniform_buffer.h"
#include "mcc/engine/engine_event.h"
#include "mcc/component/component.h"

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

    friend std::ostream& operator<<(std::ostream& stream, const PerspectiveCamera& rhs) {
      stream << "PerspectiveCamera(";
      stream << "pos=" << glm::to_string(rhs.pos) << ", ";
      stream << "front=" << glm::to_string(rhs.front) << ", ";
      stream << "up=" << glm::to_string(rhs.up) << ", ";
      stream << "right=" << glm::to_string(rhs.right) << ", ";
      stream << "world_up=" << glm::to_string(rhs.world_up) << ", ";
      stream << "yaw=" << rhs.yaw << ", ";
      stream << "pitch=" << rhs.pitch << ", ";
      stream << "speed=" << rhs.speed << ", ";
      stream << "sensitivity=" << rhs.sensitivity << ", ";
      stream << "zoom=" << rhs.zoom;
      stream << ")";
      return stream;
    }
  };

  static constexpr const glm::vec3 kDefaultCameraPosition = glm::vec3(2.0f, 2.0f, 0.0f);

  class PerspectiveCameraComponent : public StatefulComponent<PerspectiveCamera> {
  protected:
    rx::subscription on_mouse_move_;
    rx::subscription on_mouse_init_;
    rx::subscription on_mouse_deinit_;
    rx::subscription on_engine_tick_;

    PerspectiveCameraComponent();
    void OnMouseMove(mouse::MouseMoveEvent* event);
    void OnMouseCreated(mouse::MouseCreatedEvent* event);
    void OnMouseDestroyed(mouse::MouseDestroyedEvent* event);
    void OnTick(engine::TickEvent* event);
  public:
    ~PerspectiveCameraComponent() override {
      if(on_mouse_move_.is_subscribed())
        on_mouse_move_.unsubscribe();
      if(on_mouse_init_.is_subscribed())
        on_mouse_init_.unsubscribe();
      if(on_mouse_deinit_.is_subscribed())
        on_mouse_deinit_.unsubscribe();
      if(on_engine_tick_.is_subscribed())
        on_engine_tick_.unsubscribe();
    }

    DECLARE_COMPONENT(PerspectiveCamera);
  public:
    static void Init();
    static PerspectiveCameraComponent* Get();
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