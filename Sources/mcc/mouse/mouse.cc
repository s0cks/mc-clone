#include "mcc/mouse/mouse.h"

#include <units.h>

#include "mcc/uv_utils.h"
#include "mcc/thread_local.h"
#include "mcc/window/window.h"
#include "mcc/engine/engine.h"

namespace mcc {
  namespace mouse {
    static ThreadLocal<Mouse> mouse_;
    static MouseEventSubject events_;

    template<typename E>
    static inline void
    Publish(Mouse* mouse) {
      E event(mouse);
      return events_.get_subscriber().on_next(&event);
    }

    static inline void
    SetMouse(Mouse* mouse) {
      mouse_.Set(mouse);
    }

    static inline void
    InitializeMouse(engine::Engine* engine, Window* window) {
      MCC_ASSERT(engine);
      MCC_ASSERT(window);

      LOG(INFO) << "initializing mouse.....";
#ifdef MCC_DEBUG
      using namespace units::time;
      const auto startns = uv_hrtime();
#endif//MCC_DEBUG

      const auto mouse = new GlfwMouse(engine, window);
      Publish<MouseInitializedEvent>(mouse);
      SetMouse(mouse);

#ifdef MCC_DEBUG
      const auto stopns = uv_hrtime();
      const auto totalns = nanosecond_t(stopns - startns);
      LOG(INFO) << "mouse initialized in " << totalns;
#else
      LOG(INFO) << "mouse initialized.";
#endif //MCC_DEBUG
    }

    static inline void
    DeinitializeMouse() {
      LOG(INFO) << "deinitializing mouse.....";
#ifdef MCC_DEBUG
      using namespace units::time;
      const auto startns = uv_hrtime();
#endif//MCC_DEBUG

      const auto mouse = GetMouse();
      Publish<MouseDeinitializedEvent>(mouse);
      delete mouse;
      SetMouse(nullptr);

#ifdef MCC_DEBUG
      const auto stopns = uv_hrtime();
      const auto totalns = nanosecond_t(stopns - startns);
      LOG(INFO) << "mouse initialized in " << totalns;
#else
      LOG(INFO) << "mouse initialized.";
#endif //MCC_DEBUG
    }

    void InitMouse() {
      const auto window = Window::Get();
      if(!window) {
        LOG(ERROR) << "no window available to create mouse.";
        return;
      }

      window->OnOpened()
        .subscribe([](WindowEvent* event) {
          const auto engine = engine::Engine::GetEngine();
          InitializeMouse(engine, event->window());
        });
    }

    bool HasMouse() {
      return mouse_.Get() != nullptr;
    }

    Mouse* GetMouse() {
      return mouse_.Get();
    }

    MouseEventObservable OnEvent() {
      return events_.get_observable();
    }
  }
}