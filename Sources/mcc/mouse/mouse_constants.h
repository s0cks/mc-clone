#ifndef MCC_MOUSE_CONSTANTS_H
#define MCC_MOUSE_CONSTANTS_H

#include <iostream>
#include "mcc/gfx.h"

namespace mcc::mouse {
  struct MousePosition {
    glm::vec2 pos;
    glm::vec2 delta;

    friend std::ostream& operator<<(std::ostream& stream, const MousePosition& rhs) {
      stream << "MousePosition(";
      stream << "pos=" << glm::to_string(rhs.pos) << ", ";
      stream << "delta=" << glm::to_string(rhs.delta);
      stream << ")";
      return stream;
    }
  };
}

#endif //MCC_MOUSE_CONSTANTS_H