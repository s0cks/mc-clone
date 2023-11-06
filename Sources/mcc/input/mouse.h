#ifndef MCC_MOUSE_H
#define MCC_MOUSE_H

#include "mcc/rx.h"
#include "mcc/gfx.h"
#include "mcc/ecs/entity.h"
#include "mcc/input/constants.h"

namespace mcc {
    struct MousePosition {
    glm::vec2 pos;
    glm::vec2 delta;

    friend std::ostream& operator<<(std::ostream& stream, const MousePosition& rhs) {
      stream << "MousePosition(";
      stream << "pos=" << glm::to_string(rhs.pos) << ", ";
      stream << "delta=" << glm::to_string(rhs.delta);
      stream << ")";
      return stream;
    }
  };

  enum MouseButton : uint8_t {
    kButton1 = GLFW_MOUSE_BUTTON_1,
    kButton2 = GLFW_MOUSE_BUTTON_2,
    kButton3 = GLFW_MOUSE_BUTTON_3,
    kButton4 = GLFW_MOUSE_BUTTON_4,
    kButton5 = GLFW_MOUSE_BUTTON_5,
    kButton6 = GLFW_MOUSE_BUTTON_6,
    kButton7 = GLFW_MOUSE_BUTTON_7,
    kButton8 = GLFW_MOUSE_BUTTON_8,
    kNumberOfMouseButtons = (GLFW_MOUSE_BUTTON_LAST + 1),
    kLeftButton = GLFW_MOUSE_BUTTON_LEFT,
    kRightButton = GLFW_MOUSE_BUTTON_RIGHT,
    kMiddleButton = GLFW_MOUSE_BUTTON_MIDDLE,
  };
  
  struct MouseButtonEvent {
    MouseButton button;
    InputState state;
  };

  typedef rxsub::subject<MouseButtonEvent> MouseButtonEventSubject;

  class Mouse {
    DEFINE_NON_INSTANTIABLE_TYPE(Mouse);
  public:
    static glm::vec2 GetPosition();
    static glm::vec2 GetNormalizedPosition();
    static glm::vec2 GetDelta();
    static glm::vec3 CastRay();
    static std::optional<Entity> CastRayTo(const float diff = 0.5);
    static void Process();
    static rxsub::subject<MousePosition>& GetPositionSubject();
    static rxsub::subject<MouseButtonEvent>& GetButtonEventSubject();
  };
}

#endif //MCC_MOUSE_H