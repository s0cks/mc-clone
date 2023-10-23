#ifndef MCC_KEYBOARD_CONSTANTS_H
#define MCC_KEYBOARD_CONSTANTS_H

#include <functional>
#include "mcc/gfx.h"

namespace mcc::keyboard {
  typedef std::function<void()> KeyCallback;

  enum KeyState : uint8_t {
    kPressed = GLFW_PRESS,
    kReleased = GLFW_RELEASE,
    kNumberOfKeyStates = 2,
  };
  
#define FOR_EACH_KEY_CODE(_) \
  _(Space, SPACE) \
  _(Apostrophe, APOSTROPHE) \
  _(Comma, COMMA) \
  _(Minus, MINUS) \
  _(Period, PERIOD) \
  _(Slash, SLASH) \
  _(0, 0) \
  _(1, 1) \
  _(2, 2) \
  _(3, 3) \
  _(4, 4) \
  _(5, 5) \
  _(6, 6) \
  _(7, 7) \
  _(8, 8) \
  _(9, 9) \
  _(Semicolon, SEMICOLON) \
  _(Equal, EQUAL) \
  _(A, A) \
  _(B, B) \
  _(C, C) \
  _(D, D) \
  _(E, E) \
  _(F, F) \
  _(G, G) \
  _(H, H) \
  _(I, I) \
  _(J, J) \
  _(K, K) \
  _(L, L) \
  _(M, M) \
  _(N, N) \
  _(O, O) \
  _(P, P) \
  _(Q, Q) \
  _(R, R) \
  _(S, S) \
  _(T, T) \
  _(U, U) \
  _(V, V) \
  _(W, W) \
  _(X, X) \
  _(Y, Y) \
  _(Z, Z) \
  _(LeftBracket, LEFT_BRACKET) \
  _(Backslash, BACKSLASH) \
  _(RightBracket, RIGHT_BRACKET) \
  _(Grave, GRAVE_ACCENT) \
  _(Escape, ESCAPE) \
  _(Enter, ENTER) \
  _(Tab, TAB) \
  _(Backspace, BACKSPACE) \
  _(Insert, INSERT) \
  _(Delete, DELETE) \
  _(Right, RIGHT) \
  _(Left, LEFT) \
  _(Down, DOWN) \
  _(Up, UP) \
  _(PageUp, PAGE_UP) \
  _(PageDown, PAGE_DOWN) \
  _(Home, HOME) \
  _(End, END) \
  _(CapsLock, CAPS_LOCK) \
  _(ScrollLock, SCROLL_LOCK) \
  _(NumLock, NUM_LOCK) \
  _(PrintScreen, PRINT_SCREEN) \
  _(Pause, PAUSE)

  enum KeyCode : uint32_t {
    kKeyUnknown = 0,
#define DEFINE_KEY_CODE(Name, Binding) k##Name,
    FOR_EACH_KEY_CODE(DEFINE_KEY_CODE)
#undef DEFINE_KEY_CODE
    kNumberOfKeyCodes,
  };
}

#endif //MCC_KEYBOARD_CONSTANTS_H