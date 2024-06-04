#include "mcc/mouse/mouse.h"

#include "mcc/transform.h"
#include "mcc/window/window.h"
#include "mcc/engine/engine.h"

#include "mcc/mouse/mouse_flags.h"

namespace mcc::mouse {
  GlfwMouse::GlfwMouse(engine::Engine* engine, Window* window):
    Mouse(engine),
    window_(window),
    buttons_(),
    states_(),
    pos_(),
    last_pos_(),
    delta_() {
    glfwSetInputMode(window->handle(), GLFW_CURSOR, kDefaultInputMode);
    for(auto idx = 0; idx < kNumberOfMouseButtons; idx++)
      buttons_.insert(MouseButton(idx));
  }

  GlfwMouse::InputMode GlfwMouse::GetInputMode() const {
    return static_cast<GlfwMouse::InputMode>(glfwGetInputMode(GetWindowHandle(), GLFW_CURSOR));
  }

  static inline glm::vec2
  CalculateNDC(const glm::vec2 pos) {
    const auto window = Window::Get();
    MCC_ASSERT(window);
    const auto size = window->GetSize();
    const auto mX = pos.x / (size[0] / 2) - 1;
    const auto mY = -1 * (pos.y / (size[1] / 2)) - 1;
    const auto ndc = glm::vec2(mX, mY);
    DLOG(INFO) << glm::to_string(pos) << " (pos) => " << glm::to_string(ndc) << " (ndc).";
    return ndc;
  }

  void GlfwMouse::SetCursorPos(const glm::vec2& pos) {
    const auto window = Window::Get();
    glfwSetCursorPos(window->handle(), pos[0], pos[1]);
  }

  glm::vec2 GlfwMouse::GetNormalizedPosition() const {
    return CalculateNDC(GetCursorPosition());
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
    const auto window = Window::Get();
    if(!window || !window->IsFocused())
      return; // skip

    const auto window_size = window->GetSize();
    const auto handle = Window::Get()->handle();
    pos_ = GetCursorPosition();
    if(HasMotion(last_pos_ - pos_))
      PublishEvent<MouseMoveEvent>(this, pos_, last_pos_);
    last_pos_ = pos_;

    for(const auto& btn : buttons_) {
      const auto current_state = states_[btn];
      const auto new_state = btn.GetState(GetWindowHandle());
      if(new_state != current_state) {
        states_[btn] = new_state;
        switch(new_state) {
          case MouseButton::kPressed:
            PublishEvent<MouseButtonPressedEvent>(this, CalculateNDC(pos_), btn);
            continue;
          case MouseButton::kReleased:
            PublishEvent<MouseButtonReleasedEvent>(this, CalculateNDC(pos_), btn);
            continue;
          default:
            continue;
        }
      }
    }
  }
}