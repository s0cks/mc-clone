#include "mcc/mouse/mouse.h"

#include "mcc/transform.h"
#include "mcc/window/window.h"
#include "mcc/engine/engine.h"
#include "mcc/camera/perspective_camera.h"

namespace mcc::mouse {
  GlfwMouse::GlfwMouse(engine::Engine* engine, Window* window):
    Mouse(engine),
    window_(window),
    buttons_(),
    states_(),
    pos_(),
    last_pos_(),
    delta_() {
    for(auto idx = 0; idx < kNumberOfMouseButtons; idx++)
      buttons_.insert(MouseButton(idx));
  }

  glm::vec2 GlfwMouse::GetNormalizedPosition() const {
    const auto size = Window::Get()->GetSize();
    return glm::vec2(2.0f * pos_.x / size[0] - 1.0f, 1.0f - 2.0f * pos_.y / size[1]);
  }

  glm::vec2 GlfwMouse::GetCursorPosition() const {
    const auto window = Window::Get();
    double xPos, yPos;
    glfwGetCursorPos(window->handle(), &xPos, &yPos);
    return glm::vec2(static_cast<float>(xPos), static_cast<float>(yPos));
  }
  
  static inline bool
  HasNoMotion(const glm::vec2& delta) {
    return delta[0] == 0.0f && delta[1] == 0.0f;
  }

  static inline bool
  HasMotion(const glm::vec2& delta) {
    return !HasNoMotion(delta);
  }

  GLFWwindow* GlfwMouse::GetWindowHandle() const {
    return GetWindow()->handle();
  }

  void GlfwMouse::OnPreTick(engine::PreTickEvent* event) {
    const auto handle = Window::Get()->handle();
    pos_ = GetCursorPosition();
    delta_ = (pos_ - last_pos_);
    if(HasMotion(delta_))
      PublishEvent<MouseMoveEvent>(this, pos_, last_pos_, delta_);
    last_pos_ = pos_;

    for(const auto& btn : buttons_) {
      const auto current_state = states_[btn];
      const auto new_state = btn.GetState(GetWindowHandle());
      if(new_state != current_state) {
        states_[btn] = new_state;
        switch(new_state) {
          case MouseButton::kPressed:
            PublishEvent<MouseButtonPressedEvent>(this, btn);
            continue;
          case MouseButton::kReleased:
            PublishEvent<MouseButtonReleasedEvent>(this, btn);
            continue;
          default:
            continue;
        }
      }
    }
  }
}