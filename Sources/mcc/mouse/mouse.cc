#include "mcc/mouse/mouse.h"

#include "mcc/thread_local.h"
#include "mcc/window/window.h"

#include "mcc/uv_utils.h"

#include <units.h>

namespace mcc {
  namespace mouse {
    static ThreadLocal<Mouse> mouse_;

    static inline void
    InitializeMouse(Window* window) {
      if(!window) {
        LOG(ERROR) << "no window available to create mouse.";
        return;
      }
      LOG(INFO) << "initializing mouse.....";
#ifdef MCC_DEBUG
      using namespace units::time;
      const auto startns = uv_hrtime();
#endif//MCC_DEBUG

      SetMouse(new GlfwMouse(window));

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
          InitializeMouse(event->window());
        });
    }

    void SetMouse(Mouse* mouse) {
      mouse_.Set(mouse);
    }

    bool HasMouse() {
      return mouse_.Get() != nullptr;
    }

    Mouse* GetMouse() {
      return mouse_.Get();
    }
  }
}