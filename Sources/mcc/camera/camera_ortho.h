#ifndef MCC_CAMERA_ORTHO_H
#define MCC_CAMERA_ORTHO_H

#include <glog/logging.h>

#include "mcc/glm.h"
#include "mcc/window/window.h"

namespace mcc {
  class Window;
  class OrthoCamera {
  public:
    static constexpr const glm::vec3 kDefaultPos = glm::vec3(0.0f, 0.0f, 3.0f);
    static constexpr const glm::vec3 kDefaultEyePos = glm::vec3(0.0f, 0.0f, 0.0f);
    static constexpr const glm::vec3 kDefaultUp = glm::vec3(0.0f, 1.0f, 0.0f);
  protected:
    glm::vec3 eye_;
    glm::vec3 pos_;
    glm::vec3 up_;
    glm::vec2 size_;
    glm::mat4 projection_;
    glm::mat4 view_;
    glm::mat4 viewprojection_;

    virtual void Update() {
      projection_ = CalculateProjection();
      view_ = CalculateView();
      viewprojection_ = projection_ * view_;
    }

    virtual void SetPosition(const glm::vec3 pos) {
      pos_ = pos;
      Update();
    }

    inline void SetSize(const glm::vec2& size) {
      size_ = size;
      Update();
    }

    virtual glm::mat4 CalculateView() const {
      return glm::lookAt(eye_, pos_, up_);
    }
    
    glm::mat4 CalculateProjection() const {
      return glm::ortho(0.0f, size_[0] * 1.0f, size_[1] * 1.0f, 0.0f, -1000.0f, 1000.0f);
    }
  public:
    OrthoCamera(const glm::vec2 size, const float aspect = 2.0f):
      eye_(kDefaultEyePos),
      pos_(kDefaultPos),
      up_(kDefaultUp),
      size_(size),
      projection_(CalculateProjection()),
      view_(CalculateView()),
      viewprojection_(projection_ * view_) {
      window::OnWindowSizeEvent()
        .subscribe([this](WindowSizeEvent* event) {
          SetSize(event->size());
        });
    }
    OrthoCamera(Window* window);
    virtual ~OrthoCamera() = default;

    const glm::vec2& GetSize() const {
      return size_;
    }

    const glm::vec3& GetPos() const {
      return pos_;
    }

    const glm::vec3& GetEyePos() const {
      return eye_;
    }

    const glm::vec3& GetUp() const {
      return up_;
    }

    float GetScale() const {
      return 1.0f / size_[0];
    }

    const glm::mat4& GetProjection() const {
      return projection_;
    }

    const glm::mat4& GetView() const {
      return view_;
    }

    const glm::mat4& GetViewProjection() const {
      return viewprojection_;
    }
  };
}

#endif //MCC_CAMERA_ORTHO_H