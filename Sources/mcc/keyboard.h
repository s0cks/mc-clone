#ifndef MCC_KEYBOARD_H
#define MCC_KEYBOARD_H

#include <opengl/gl3.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "mcc/uv_utils.h"

namespace mcc {
  class Window;

#define FOR_EACH_KEY_STATE(V) \
  V(Pressed, GLFW_PRESS)      \
  V(Released, GLFW_RELEASE)

  enum KeyState {
#define DEFINE_KEY_STATE(Name, GLFW) k##Name = GLFW,
    FOR_EACH_KEY_STATE(DEFINE_KEY_STATE)
#undef DEFINE_KEY_STATE
  };

  enum KeyCode : int {
    kKeyEscape = GLFW_KEY_ESCAPE,
  };

  bool IsKeyState(Window* window, const int key, const KeyState state);

#define DEFINE_KEY_STATE_CHECK(Name, _) \
  static inline bool IsKey##Name(Window* window, const int key) { return IsKeyState(window, key, KeyState::k##Name); }
  FOR_EACH_KEY_STATE(DEFINE_KEY_STATE_CHECK)
#undef DEFINE_KEY_STATE_CHECK

  typedef std::function<void()> KeyStateCallback;

  class KeyStateListener {
  private:
    uv::IdleHandle handle_;
  public:
    KeyStateListener(const KeyCode key, const KeyState state, KeyStateCallback callback);
    virtual ~KeyStateListener() = default;
  };

  class KeyPressedListener : public KeyStateListener {
  public:
    KeyPressedListener(const KeyCode key, KeyStateCallback callback):
      KeyStateListener(key, KeyState::kPressed, std::move(callback)) {
    }
    ~KeyPressedListener() override = default;
    DEFINE_NON_COPYABLE_TYPE(KeyPressedListener);
  public:
    static inline KeyPressedListener*
    New(const KeyCode key, KeyStateCallback callback) {
      return new KeyPressedListener(key, callback);
    }
  };

  class KeyReleasedListener : public KeyStateListener {
  public:
    KeyReleasedListener(const KeyCode key, KeyStateCallback callback):
      KeyStateListener(key, KeyState::kReleased, std::move(callback)) {
    }
    ~KeyReleasedListener() override = default;
  };
}

#endif //MCC_KEYBOARD_H