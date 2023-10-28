#include "mcc/gui/gui_window.h"

namespace mcc::gui {
  static Window::WindowPtr current_;

  void Window::SetCurrent(Window::WindowPtr value) {
    current_ = value;
  }

  Window::WindowPtr Window::GetCurrent() {
    return current_;
  }

  bool Window::HasCurrent() {
    return current_ != nullptr;
  }

  void Window::Render(const glm::mat4 proj, const glm::mat4 view) {
    glm::mat4 model(1.0f);
    shader_.ApplyShader();
    shader_.SetMat4("projection", proj);
    shader_.SetMat4("view", view);
    shader_.SetMat4("model", model);
    shader_.SetVec3("color", glm::vec3(1.0f, 1.0f, 1.0f));
    shader_.ApplyShader();
    mesh_->Render();
  }
}