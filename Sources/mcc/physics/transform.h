#ifndef MCC_TRANSFORM_H
#define MCC_TRANSFORM_H

#include "mcc/gfx.h"

namespace mcc::physics {
  struct Transform {
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;

    friend std::ostream& operator<<(std::ostream& stream, const Transform& rhs) {
      stream << "physics::Transform(";
      stream << "position=" << glm::to_string(rhs.position) << ", ";
      stream << "rotation=" << glm::to_string(rhs.rotation) << ", ";
      stream << "scale=" << glm::to_string(rhs.scale);
      stream << ")";
      return stream;
    }
  };
}

#endif //MCC_TRANSFORM_H