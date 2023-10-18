#ifndef MCC_CAMERA_H
#define MCC_CAMERA_H

#include <OpenGL/gl3.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/vec2.hpp>

namespace mcc {
  class Camera {
  protected:
    glm::mat4 view_;
    glm::mat4 proj_;

    Camera() = default;
  public:
    Camera(const Camera& rhs) = default;
    virtual ~Camera() = default;
    virtual bool IsPerspective() const { return false; }
    virtual bool IsOrtho() const { return false; }

    Camera& operator=(const Camera& rhs) = default;
  };

  class PerspectiveCamera : public Camera {
  public:
    static constexpr const float kDefaultYaw = -90.0f;
    static constexpr const float kDefaultPitch = 0.0f;
    static constexpr const float kDefaultSpeed = 2.5f;
    static constexpr const float kDefaultSensitivity = 0.1f;
    static constexpr const float kDefaultZoom = 45.0f;

    static constexpr const glm::vec3 kFrontVec = glm::vec3(0.0f, 0.0f, -1.0f);
    static constexpr const glm::vec3 kUpVec = glm::vec3(0.0f, 1.0f, 0.0f);
  protected:
    glm::vec3 pos_;
    glm::vec3 front_;
    glm::vec3 up_;
    glm::vec3 right_;
    glm::vec3 world_up_;
    float yaw_;
    float pitch_;
    float speed_;
    float zoom_;

    void Update();
  public:
    PerspectiveCamera(const glm::vec3 pos, const glm::vec3 up):
      Camera(),
      pos_(pos),
      front_(glm::vec3(0.0f, 0.0f, -1.0f)),
      up_(up),
      right_(),
      world_up_(up),
      pitch_(kDefaultPitch),
      yaw_(kDefaultYaw),
      speed_(kDefaultSpeed),
      zoom_(kDefaultZoom) {
      Update();
    }
    PerspectiveCamera(const float posX, const float posY, const float posZ,
                      const float upX, const float upY, const float upZ):
      PerspectiveCamera({ posX, posY, posZ }, { upX, upY, upZ }) {
    }
    PerspectiveCamera():
      PerspectiveCamera({ 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }) {
    }
    ~PerspectiveCamera() override = default;

    glm::mat4 GetViewMatrix() const {
      return glm::lookAt(pos_, pos_ + kFrontVec, kUpVec);
    }

    bool IsPerspective() const override {
      return true;
    }
  };

  class OrthoCamera : public Camera {
  public:
    glm::vec2 min_;
    glm::vec2 max_;

    static inline glm::mat4
    ComputeProjectionMatrix(glm::vec2 min, glm::vec2 max) {
      return glm::ortho(min[0], max[0], min[1], max[1]);
    }

    static void Set(OrthoCamera* camera);
  public:
    OrthoCamera(const glm::vec2 min, const glm::vec2 max):
      Camera(),
      min_(min),
      max_(max) {
    }
    explicit OrthoCamera(const glm::vec2 size):
      Camera(),
      min_(glm::vec2(0.0, 0.0)),
      max_(size) {
    }
    ~OrthoCamera() override = default;

    void SetMin(const glm::vec2 value) {
      min_ = value;
    }

    glm::vec2 GetMin() const {
      return min_;
    }

    void SetMax(const glm::vec2 value) {
      max_ = value;
    }

    glm::vec2 GetMax() const {
      return max_;
    }

    glm::mat4 GetProjectionMatrix() const {
      return ComputeProjectionMatrix(min_, max_);
    }

    inline void UpdateSize(const glm::vec2 value) {
      SetMax(value);
    }

    bool IsOrtho() const override {
      return true;
    }
  public:
    static OrthoCamera* Initialize(const glm::vec2 size);
    static OrthoCamera* Get();
    
    static inline glm::mat4
    GetCurrentProjectionMatrix() {
      const auto cam = Get();
      return cam->GetProjectionMatrix();
    }
  };
}

#endif //MCC_CAMERA_H