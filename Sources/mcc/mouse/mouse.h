#ifndef MCC_MOUSE_H
#define MCC_MOUSE_H

#include <utility>
#include <optional>

#include "mcc/rx.h"
#include "mcc/gfx.h"
#include "mcc/entity/entity.h"

#include "mcc/mouse/mouse_events.h"
#include "mcc/mouse/mouse_button.h"

#include "mcc/engine/engine_event_listener.h"

namespace mcc {
  namespace engine {
    class Engine;
  }

  namespace mouse {
    rx::observable<MouseEvent*> OnMouseEvent();

#define DEFINE_ON_MOUSE_EVENT(Name)               \
    static inline rx::observable<Name##Event*>    \
    On##Name##Event() {                           \
      return OnMouseEvent()                       \
        .filter(Name##Event::Filter)              \
        .map(Name##Event::Cast);                  \
    }
    FOR_EACH_MOUSE_EVENT(DEFINE_ON_MOUSE_EVENT)
#undef DEFINE_ON_MOUSE_EVENT

    class Mouse : public engine::PreTickEventListener {
      friend class engine::Engine;
    protected:
      explicit Mouse(engine::Engine* engine):
        engine::PreTickEventListener(engine) {
      }

      static void PublishEvent(MouseEvent* event);
      
      template<class E, typename... Args>
      static inline void
      PublishEvent(Args... args) {
        E event(args...);
        return PublishEvent((MouseEvent*) &event);
      }
    public:
      virtual ~Mouse() = default;
      virtual glm::vec2 GetCursorPosition() const = 0;
      virtual glm::vec2 GetPosition() const = 0;
      virtual glm::vec2 GetNormalizedPosition() const = 0;
      virtual glm::vec2 GetDelta() const = 0;
      virtual rx::observable<MouseButton> GetAllButtons() const = 0;
      virtual rx::observable<std::pair<MouseButton, MouseButton::State>> GetAllButtonStates() const = 0;
    };
    void InitMouse();
    Mouse* GetMouse();
  }
  using mouse::Mouse;
  using mouse::GetMouse;

#define DEFINE_USE_ON_MOUSE_EVENT(Name)     \
  using mouse::On##Name##Event;
  FOR_EACH_MOUSE_EVENT(DEFINE_USE_ON_MOUSE_EVENT)
#undef DEFINE_USE_ON_MOUSE_EVENT
}

#include "mcc/mouse/mouse_glfw.h"

#endif //MCC_MOUSE_H