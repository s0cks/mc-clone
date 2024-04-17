#ifndef MCC_MOUSE_H
#define MCC_MOUSE_H

#include <optional>
#include "mcc/rx.h"
#include "mcc/gfx.h"
#include "mcc/ecs/entity.h"
#include "mcc/mouse/mouse_event.h"

namespace mcc {
  namespace engine {
    class Engine;
  }

  namespace mouse {
    class Mouse {
      friend class engine::Engine;
    protected:
      Mouse() = default;

      virtual void Process() = 0;
    public:
      virtual ~Mouse() = default;
      virtual rx::observable<MouseEvent*> OnEvent() const = 0;
      virtual MouseButtonState GetButton(const MouseButton btn) const = 0;
      virtual glm::vec2 GetCursorPosition() const = 0;
      virtual glm::vec2 GetPosition() const = 0;
      virtual glm::vec2 GetNormalizedPosition() const = 0;
      virtual glm::vec2 GetDelta() const = 0;
      virtual glm::vec3 CastRay() const = 0;
      virtual std::optional<Entity> CastRayTo(const float diff = 0.5f) const = 0;

      inline bool IsPressed(const MouseButton btn) const {
        return GetButton(btn) == kMousePressed;
      }

      inline bool IsReleased(const MouseButton btn) const {
        return GetButton(btn) == kMouseReleased;
      }

#define DEFINE_ON_EVENT(Name)                                           \
      inline rx::observable<Name##Event*> On##Name() const {            \
        return OnEvent()                                                \
          .filter([](MouseEvent* event) {                               \
            return event->Is##Name##Event();                            \
          })                                                            \
          .map([](MouseEvent* event) {                                  \
            return event->As##Name##Event();                            \
          });                                                           \
      }
      FOR_EACH_MOUSE_EVENT(DEFINE_ON_EVENT)
#undef DEFINE_ON_EVENT

      inline rx::observable<MouseButtonEvent*> OnPressed(const MouseButton btn) const {
        return OnEvent()
          .filter([btn](MouseEvent* event) {
            return event->IsMouseButtonEvent()
                && (event->AsMouseButtonEvent()->button() == btn);
          })
          .map([](MouseEvent* event) {
            return event->AsMouseButtonEvent();
          });
      }
    };
  
    void InitMouse();
    bool HasMouse();
    Mouse* GetMouse();

    MouseEventObservable OnEvent();
    
    static inline rx::observable<MouseInitializedEvent*>
    OnInitialized() {
      return OnEvent()
        .filter([](MouseEvent* event) {
          return event->IsMouseInitializedEvent();
        })
        .map([](MouseEvent* event) {
          return event->AsMouseInitializedEvent();
        });
    }

    static inline rx::observable<MouseDeinitializedEvent*>
    OnDeinitialized() {
      return OnEvent()
        .filter([](MouseEvent* event) {
          return event->IsMouseDeinitializedEvent();
        })
        .map([](MouseEvent* event) {
          return event->AsMouseDeinitializedEvent();
        });
    }
  }

  using mouse::Mouse;
  using mouse::GetMouse;
}

#include "mcc/mouse/mouse_glfw.h"

#endif //MCC_MOUSE_H