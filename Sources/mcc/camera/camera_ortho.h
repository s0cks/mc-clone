#ifndef MCC_CAMERA_ORTHO_H
#define MCC_CAMERA_ORTHO_H

#include <utility>

#include "mcc/uri.h"
#include "mcc/keyboard/key.h"
#include "mcc/camera/camera.h"

namespace mcc::camera {
  struct OrthoCameraData {
    friend class OrthoCamera;
  public:
    glm::vec4 viewport;
    glm::vec3 eye;
    glm::vec3 up;
    glm::vec3 pos;
    glm::mat4 projection;
    glm::mat4 view;

    OrthoCameraData() = default;
    OrthoCameraData(const OrthoCameraData& rhs) = default;
    ~OrthoCameraData() = default;

    float GetViewportWidth() const {
      return viewport[2];
    }

    float GetViewportHeight() const {
      return viewport[3];
    }

    friend std::ostream& operator<<(std::ostream& stream, const OrthoCameraData& rhs) {
      return stream;
    }
  private:
    OrthoCameraData(const glm::vec2& viewport_size,
                    const glm::vec3& e,
                    const glm::vec3& p,
                    const glm::vec3& u):
      viewport(0, 0, viewport_size),
      eye(e),
      pos(p),
      up(u),
      projection(CalculateProjection()),
      view(CalculateView()) {
    }

    glm::mat4 CalculateView() const;
    glm::mat4 CalculateProjection() const;
  };

  class OrthoCamera : public Camera {
    friend class GlfwMouse;
  public:
    static constexpr const auto kDefaultUp = glm::vec3(0.0f, 1.0f, 0.0f);
    static constexpr const auto kDefaultPos = glm::vec3(1.0f, 0.0f, 0.0f);
    static constexpr const auto kFront = glm::vec3(0.0f, 0.0f, -1.0f);
  protected:
    OrthoCameraData data_;

    glm::vec2 CalculateMouseNDC(const glm::vec3& pos, const glm::mat4& model = glm::mat4(1.0f)) const;

    inline glm::vec2
    CalculateMouseNDC(const glm::vec2& pos, const glm::mat4& model = glm::mat4(1.0f)) const {
      return CalculateMouseNDC(glm::vec3(pos, 0.0f), model);
    }

    inline OrthoCameraData& data() {
      return data_;
    }

    inline const OrthoCameraData& data() const {
      return data_;
    }

    void MoveCamera(const glm::vec3& dir);
    void OnKeyPressed(const keyboard::KeyCode code);
  public:
    OrthoCamera(const glm::vec3& eye,
                const glm::vec3& pos,
                const glm::vec3& up,
                const glm::vec2& viewport_size);
    OrthoCamera(const glm::vec3& eye,
                const glm::vec3& pos,
                const glm::vec3& up);
    OrthoCamera(const glm::vec3& pos = kDefaultPos,
                const glm::vec3& up = kDefaultUp):
      OrthoCamera(pos, pos, up) {
    }
    ~OrthoCamera() override = default;
    std::string ToString() const override;

    const glm::vec4& GetViewport() const {
      return data_.viewport;
    }

    const glm::vec3& GetEye() const {
      return data_.eye;
    }

    const glm::vec3& GetUp() const {
      return data_.up;
    }

    const glm::vec3& GetPos() const {
      return data_.pos;
    }

    const glm::mat4& GetProjection() const override {
      return data_.projection;
    }
    
    const glm::mat4& GetView() const override {
      return data_.view;
    }

    std::pair<glm::vec3, glm::vec3> CastRay(const glm::vec2& mpos, const float distance = 15) const;
    glm::vec3 CastRayAt(const glm::vec2& mpos) const;
  public:
    static OrthoCamera* New(const Metadata& meta, const OrthoCameraData& data);
    
    static inline OrthoCamera*
    New(const OrthoCameraData& data) {
      return New(Metadata(), data);
    }

    static OrthoCamera* FromJson(const uri::Uri& uri);
    static OrthoCamera* FromJson(const std::string& value);
  };

  OrthoCamera* GetOrthoCamera();
  void SetOrthoCamera(OrthoCamera* camera);
}

#endif //MCC_CAMERA_ORTHO_H