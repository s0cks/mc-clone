#ifndef MCC_RIGID_BODY_H
#define MCC_RIGID_BODY_H

#include "mcc/gfx.h"
#include "mcc/component/component.h"
#include "mcc/component/component_state_table.h"

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

    DECLARE_COMPONENT(RigidBody);
  private:
    static void OnPreInit();
    static void OnInit();
    static void OnPostInit();
  public:
    static void Init();
  };
}

#endif //MCC_RIGID_BODY_H