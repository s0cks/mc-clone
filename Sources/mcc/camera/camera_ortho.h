#ifndef MCC_CAMERA_ORTHO_H
#define MCC_CAMERA_ORTHO_H

#include <glog/logging.h>

#include "mcc/glm.h"

namespace mcc {
  class Window;
  class OrthoCamera {
  protected:
    glm::vec2 size_;
    float aspect_;
    glm::vec3 position_;
    glm::mat4 projection_;
    glm::mat4 view_;
    glm::mat4 viewprojection_;

    virtual void Update() {
      view_ = CalculateView();
      viewprojection_ = projection_ * view_;
    }

    virtual void SetPosition(const glm::vec3 pos) {
      position_ = pos;
      Update();
    }

    virtual glm::mat4 CalculateView() const {
      const auto eye = glm::vec3(0, 0, 1);
      const auto center = position_;
      const auto up = glm::vec3(0, 1, 0);
      return glm::lookAt(eye, center, up);
    }
    
    glm::mat4 CalculateProjection() const {
      return glm::ortho(0.0f, size_[0] * 1.0f, 0.0f, size_[1] * 1.0f, -1000.0f, 1000.0f);
    }
  public:
    OrthoCamera(const glm::vec2 size, const float aspect = 2.0f):
      size_(size),
      aspect_(aspect),
      position_(0, 0, 0),
      projection_(CalculateProjection()),
      view_(CalculateView()),
      viewprojection_(projection_ * view_) {
    }
    OrthoCamera(Window* window);
    virtual ~OrthoCamera() = default;

    const glm::vec2& GetSize() const {
      return size_;
    }

    const glm::vec3& GetPosition() const {
      return position_;
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