#ifndef MCC_INPUT_CONSTANTS_H
#define MCC_INPUT_CONSTANTS_H

#include "mcc/gfx.h"

namespace mcc {
  enum InputState : int {
    kReleased = GLFW_RELEASE,
    kPressed = GLFW_PRESS,
    kNumberOfKeyStates = 2,
  };
}

#endif //MCC_INPUT_CONSTANTS_H