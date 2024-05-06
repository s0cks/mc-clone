#ifndef MCC_CAMERA_PERSPECTIVE_H
#define MCC_CAMERA_PERSPECTIVE_H

#include <ostream>

#include "mcc/rx.h"
#include "mcc/uv/utils.h"
#include "mcc/mouse/mouse.h"
#include "mcc/camera/camera.h"
#include "mcc/keyboard/keyboard.h"

namespace mcc::camera {
  struct PerspectiveCameraData {
    glm::vec4 viewport;
    glm::vec3 up;
    glm::vec3 pos;
    glm::vec3 front;
    glm::vec3 right;
    glm::mat4 projection;
    glm::mat4 view;
    float yaw;
    float pitch;
    float speed;
    float sensitivity;
    float zoom;

    PerspectiveCameraData();
    PerspectiveCameraData(const glm::vec2& viewport_size,
                          const glm::vec3& f,
                          const glm::vec3& p,
                          const glm::vec3& u);
    PerspectiveCameraData(const PerspectiveCameraData& rhs) = default;
    ~PerspectiveCameraData() = default;

    friend std::ostream& operator<<(std::ostream& stream, const PerspectiveCameraData& rhs) {
      stream << "PerspectiveCameraData(";
      stream << ")";
      return stream;
    }

    friend class PerspectiveCamera;
  private:
    float CalculateVelocity(const uint64_t dts) const {
      return (speed * 100.0f) * ((1.0f * dts) / NSEC_PER_SEC);
    }

    glm::mat4 CalculateView() const;
    glm::mat4 CalculateProjection() const;
  };

  class PerspectiveCamera : public Camera {
  public:
    static constexpr const auto kDefaultFront = glm::vec3(0.0f, 0.0f, -1.0f);
    static constexpr const auto kDefaultPos = glm::vec3(0.0f);
    static constexpr const auto kDefaultUp = glm::vec3(0.0f, 1.0f, 0.0f);
  protected:
    PerspectiveCameraData data_;
    rx::subscription on_key_pressed_;
    rx::subscription on_mouse_moved_;

    void Update();
    void OnKeyPressed(const keyboard::KeyCode code);
    void OnMouseMoved(const mouse::MouseMoveEvent* event);
  public:
    explicit PerspectiveCamera(const PerspectiveCameraData& data);
    PerspectiveCamera();
    ~PerspectiveCamera() override;
    std::string ToString() const override;

    const PerspectiveCameraData& GetData() const {
      return data_;
    }

    float GetYaw() const {
      return data_.yaw;
    }

    float GetPitch() const {
      return data_.pitch;
    }

    float GetSpeed() const {
      return data_.speed;
    }

    float GetSensitivity() const {
      return data_.sensitivity;
    }

    float GetZoom() const {
      return data_.zoom;
    }

    const glm::mat4& GetProjection() const override {
      return data_.projection;
    }

    const glm::mat4& GetView() const override {
      return data_.view;
    }
  };
  
  PerspectiveCamera* GetPerspectiveCamera();
}

#endif //MCC_CAMERA_PERSPECTIVE_H