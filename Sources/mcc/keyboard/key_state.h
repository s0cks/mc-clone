#ifndef MCC_KEY_STATE_H
#define MCC_KEY_STATE_H

#include "mcc/gfx.h"

namespace mcc::keyboard {
#define FOR_EACH_KEY_STATE(V) \
  V(Pressed, GLFW_PRESS)      \
  V(Released, GLFW_RELEASE)   \
  V(Repeat, GLFW_REPEAT)

  enum KeyState : GLenum {
#define DEFINE_KEY_STATE(Name, GlValue) kKey##Name = (GlValue),
    FOR_EACH_KEY_STATE(DEFINE_KEY_STATE)
#undef DEFINE_KEY_STATE
  };

  static inline constexpr const char*
  ToString(const KeyState& rhs) {
    switch(rhs) {
#define DEFINE_TO_STRING(Name, GlValue) \
      case kKey##Name: return #Name;
      FOR_EACH_KEY_STATE(DEFINE_TO_STRING)
#undef DEFINE_TO_STRING
      default: return "Unknown KeyState";
    }
  }

  static inline std::ostream&
  operator<<(std::ostream& stream, const KeyState& rhs) {
    return stream << ToString(rhs);
  }
}

#endif //MCC_KEY_STATE_H