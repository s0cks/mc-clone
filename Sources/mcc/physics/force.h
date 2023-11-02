#ifndef MCC_PHYSICS_FORCE_H
#define MCC_PHYSICS_FORCE_H

#include "mcc/gfx.h"
#include "mcc/ecs/component.h"

namespace mcc::physics {
  struct Force {
    glm::vec3 force;

    friend std::ostream& operator<<(std::ostream& stream, const Force& rhs) {
      stream << "physics::Force(";
      stream << "force=" << glm::to_string(rhs.force);
      stream << ")";
      return stream;
    }

    DECLARE_COMPONENT(Force);
  private:
    static void OnPreInit();
    static void OnInit();
    static void OnPostInit();
  public:
    static void Init();
  };
}

#endif //MCC_PHYSICS_FORCE_H