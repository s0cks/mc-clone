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
    glm::vec2 size_;

    Camera(const glm::vec2 size):
      size_(size) {
    }
  public:
    Camera(const Camera& rhs) = default;
    virtual ~Camera() = default;
    virtual bool IsPerspective() const { return false; }
    virtual bool IsOrtho() const { return false; }

    void SetSize(const glm::vec2 value) {
      size_ = value;
    }

    glm::mat4 GetViewMatrix() const {
      glm::mat4 view = glm::mat4(1.0f);
      view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
      return view;
    }

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
    float fov_;
    float yaw_;
    float pitch_;
    float speed_;
    float zoom_;

    void Update();

    static void Set(PerspectiveCamera* camera);
  public:
    PerspectiveCamera(const glm::vec2 size,
                      const glm::vec3 pos, 
                      const glm::vec3 up):
      Camera(size),
      pos_(pos),
      front_(glm::vec3(0.0f, 0.0f, -1.0f)),
      up_(up),
      right_(),
      fov_(45.0f),
      world_up_(up),
      pitch_(kDefaultPitch),
      yaw_(kDefaultYaw),
      speed_(kDefaultSpeed),
      zoom_(kDefaultZoom) {
    }
    ~PerspectiveCamera() override = default;

    glm::mat4 GetProjectionMatrix() const {
      return glm::perspective(glm::radians(fov_), size_[0] / size_[1], 0.1f, 100.0f);
    }

    bool IsPerspective() const override {
      return true;
    }
  public:
    static PerspectiveCamera* Initialize(const glm::vec2 size);
    static PerspectiveCamera* Get();

    static inline glm::mat4
    GetCurrentProjectionMatrix() {
      return Get()->GetProjectionMatrix();
    }
  };

  class OrthoCamera : public Camera {
  public:
    glm::vec2 min_;
    
    static inline glm::mat4
    ComputeProjectionMatrix(glm::vec2 min, glm::vec2 max) {
      return glm::ortho(min[0], max[0], min[1], max[1]);
    }

    static void Set(OrthoCamera* camera);
  public:
    OrthoCamera(const glm::vec2 min, const glm::vec2 max):
      Camera(max),
      min_(min) {
    }
    explicit OrthoCamera(const glm::vec2 size):
      OrthoCamera(glm::vec2(0.0f, 0.0f), size) {
    }
    ~OrthoCamera() override = default;

    void SetMin(const glm::vec2 value) {
      min_ = value;
    }

    glm::vec2 GetMin() const {
      return min_;
    }

    void SetMax(const glm::vec2 value) {
      size_ = value;
    }

    glm::vec2 GetMax() const {
      return size_;
    }

    glm::mat4 GetProjectionMatrix() const {
      return ComputeProjectionMatrix(min_, size_);
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