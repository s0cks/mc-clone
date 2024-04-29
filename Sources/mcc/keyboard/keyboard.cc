#include "mcc/keyboard/keyboard.h"

#include "mcc/uv/utils.h"
#include "mcc/thread_local.h"
#include "mcc/window/window.h"

namespace mcc::keyboard {
  rx::subject<KeyboardEvent*> events_;
  static ThreadLocal<Keyboard> keyboard_;

  template<typename E>
  static inline void
  Publish(Keyboard* keyboard) {
    E event(keyboard);
    return events_.get_subscriber().on_next(&event);
  }

  static inline void
  SetKeyboard(Keyboard* keyboard) {
    keyboard_.Set(keyboard);
  }

  static inline void
  CreateKeyboard(engine::Engine* engine, Window* window) {
    MCC_ASSERT(engine);
    MCC_ASSERT(window);

    LOG(INFO) << "creating keyboard....";
#ifdef MCC_DEBUG
    using namespace units::time;
    const auto startns = uv_hrtime();
#endif //MCC_DEBUG

    const auto keyboard = new GlfwKeyboard(engine, window);
    Publish<KeyboardCreatedEvent>(keyboard);
    SetKeyboard(keyboard);

#ifdef MCC_DEBUG
    const auto stopns = uv_hrtime();
    const auto totalns = nanosecond_t(stopns - startns);
    LOG(INFO) << "keyboard created in " << totalns;
#endif //MCC_DEBUG
  }

  static inline void
  DestroyKeyboard() {
    LOG(INFO) << "destroying keyboard....";
#ifdef MCC_DEBUG
    using namespace units::time;
    const auto startns = uv_hrtime();
#endif //MCC_DEBUG

    const auto keyboard = GetKeyboard();
    Publish<KeyboardDestroyedEvent>(keyboard);
    delete keyboard;
    SetKeyboard(nullptr);

#ifdef MCC_DEBUG
    const auto stopns = uv_hrtime();
    const auto totalns = nanosecond_t(stopns - startns);
    LOG(INFO) << "keyboard destroyed in " << totalns;
#endif //MCC_DEBUG
  }

  void InitKeyboard() {
    const auto window = Window::Get();
    if(!window) {
      LOG(ERROR) << "no window available to create keyboard.";
      return;
    }

    window::OnWindowOpenedEvent()
      .subscribe([](WindowOpenedEvent* event) {
        const auto engine = engine::Engine::GetEngine();
        CreateKeyboard(engine, event->window());
      });
  }

  void Keyboard::PublishEvent(KeyboardEvent* event) {
    MCC_ASSERT(event);
    DLOG(INFO) << "publishing: " << event->ToString();
    const auto& subscriber = events_.get_subscriber();
    subscriber.on_next(event);
  }

  rx::observable<KeyboardEvent*> Keyboard::OnEvent() const {
    return events_.get_observable();
  }

  bool HasKeyboard() {
    return keyboard_.Get() != nullptr;
  }

  Keyboard* GetKeyboard() {
    return keyboard_.Get();
  }

  rx::observable<KeyboardEvent*> OnEvent() {
    return events_.get_observable();
  }
}