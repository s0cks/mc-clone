#ifndef MCC_PHYSICS_H
#define MCC_PHYSICS_H

#include "mcc/common.h"
#include "mcc/gfx.h"
#include "mcc/engine/tick.h"

namespace mcc::physics {
  class PhysicsSimulator {
    DEFINE_NON_INSTANTIABLE_TYPE(PhysicsSimulator);
  private:
    static void OnPreInit();
    static void OnInit();
    static void OnPostInit();
    static void OnTick(const Tick& tick);
  public:
    static void Init();

    static glm::vec3 GetGravity();
    static void SetGravity(const glm::vec3& value);
  };
}

#endif //MCC_PHYSICS_H