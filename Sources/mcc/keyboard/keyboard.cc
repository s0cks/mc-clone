#include "mcc/keyboard/keyboard.h"

#include "mcc/window.h"
#include "mcc/render_loop.h"
#include "mcc/keyboard/keyboard_trie.h"

namespace mcc::keyboard {
  static RootNode* root_ = nullptr;

  static inline KeyCode
  MapKeyCode(const int key) {    
    switch(key) {
#define DEFINE_MATCH(Name, Binding) \
      case GLFW_KEY_##Binding:      \
        return KeyCode::k##Name;
      FOR_EACH_KEY_CODE(DEFINE_MATCH)
#undef DEFINE_MATCH
      default:
        return KeyCode::kKeyUnknown;
    }
  }

  static inline void
  OnKey(GLFWwindow* window, int key, int scancode, int action, int mods) {
    static const auto code = MapKeyCode(key);
    static const auto state = static_cast<KeyState>(action);
    const auto subscription = Keyboard::Subscription::New(code, state);
    root_->Call(subscription);
  }

  void Keyboard::Initialize() {
    root_ = new RootNode();
    const auto window = Window::GetWindow();
    const auto handle = window->handle();
    glfwSetKeyCallback(handle, &OnKey);
  }

  Keyboard::Subscription Keyboard::Register(const Subscription::SubscriptionId id, KeyCode code, const KeyState state, KeyCallback callback) {
    const auto subscription = Keyboard::Subscription::New(id, code, state);
    root_->Register(subscription, callback);
    return subscription;
  }

  void Keyboard::Deregister(const Keyboard::Subscription& subscription) {
    root_->Deregister(subscription);
  }
}