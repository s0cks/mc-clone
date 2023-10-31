#ifndef MCC_RIGID_BODY_H
#define MCC_RIGID_BODY_H

#include "mcc/gfx.h"

namespace mcc::physics {
  struct RigidBody {
    glm::vec3 velocity;
    float mass;

    friend std::ostream& operator<<(std::ostream& stream, const RigidBody& rhs) {
      stream << "physics::RigidBody(";
      stream << "velocity=" << glm::to_string(rhs.velocity) << ", ";
      stream << "mass=" << rhs.mass;
      stream << ")";
      return stream;
    }
  };
}

#endif //MCC_RIGID_BODY_H