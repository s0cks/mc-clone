#ifndef MCC_MOUSE_H
#error "Please #include <mcc/mouse/mouse.h> instead."
#endif //MCC_MOUSE_H

#ifndef MCC_MOUSE_GLFW_H
#define MCC_MOUSE_GLFW_H

#include <optional>

#include "mcc/mouse/mouse.h"

namespace mcc {
  class Window;
}

namespace mcc::mouse {
  class GlfwMouse : public Mouse {
    friend class Engine;
  protected:
    Window* window_;
    MouseButtonSet buttons_;
    MouseButtonStateSet states_;
    glm::vec2 pos_;
    glm::vec2 last_pos_;
    glm::vec2 delta_;

    void OnPreTick(engine::PreTickEvent* event) override;

    inline Window* GetWindow() const {
      return window_;
    }

    GLFWwindow* GetWindowHandle() const;
  public:
    explicit GlfwMouse(engine::Engine* engine, Window* window);
    ~GlfwMouse() override = default;

    inline Window* window() const {
      return window_;
    }

    void SetCursorPos(const glm::vec2& pos) override;

    glm::vec2 GetPosition() const override {
      return pos_;
    }

    glm::vec2 GetDelta() const override {
      return delta_;
    }

    glm::vec2 GetCursorPosition() const override;
    glm::vec2 GetNormalizedPosition() const override;

    rx::observable<MouseButton> GetAllButtons() const override {
      return rx::observable<>::iterate(buttons_);
    }

    rx::observable<std::pair<MouseButton, MouseButton::State>> GetAllButtonStates() const override {
      return GetAllButtons()
        .map([this](const MouseButton& btn) {
          return std::make_pair(btn, btn.GetState(GetWindowHandle()));
        });
    }
  };
}

#endif //MCC_MOUSE_GLFW_H