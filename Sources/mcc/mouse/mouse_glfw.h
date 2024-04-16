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
    MouseEventSubject events_;
    glm::vec2 pos_;
    glm::vec2 last_pos_;
    glm::vec2 delta_;
    MouseButtonState buttons_[kNumberOfMouseButtons];

    template<class E, typename... Args>
    inline void Publish(Args... args) {
      E event(this, args...);
      return events_.get_subscriber().on_next(&event);
    }

    void Process() override;
  public:
    explicit GlfwMouse(Window* window):
      Mouse(),
      window_(window),
      pos_(),
      last_pos_(),
      delta_(),
      buttons_(),
      events_() {
    }
    ~GlfwMouse() override = default;

    inline Window* window() const {
      return window_;
    }

    rx::observable<MouseEvent*> OnEvent() const override {
      return events_.get_observable();
    }

    MouseButtonState GetButton(const MouseButton btn) const override;

    glm::vec2 GetPosition() const override {
      return pos_;
    }

    glm::vec2 GetDelta() const override {
      return delta_;
    }

    glm::vec2 GetCursorPosition() const override;
    glm::vec2 GetNormalizedPosition() const override;
    glm::vec3 CastRay() const override;
    std::optional<Entity> CastRayTo(const float diff = 0.5f) const override;
  };
}

#endif //MCC_MOUSE_GLFW_H