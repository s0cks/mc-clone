#ifndef MCC_PARTICLES_H
#define MCC_PARTICLES_H

#include "mcc/engine/tick.h"
#include "mcc/particle/particle.h"

namespace mcc::particle {
  class Particles {
    DEFINE_NON_INSTANTIABLE_TYPE(Particles);
  private:
    static void OnPreInit();
    static void OnInit();
    static void OnPostInit();
  public:
    static void Init();
    static void UpdateAll(const Tick& tick);
    static void RenderAll();
  };
}

#endif //MCC_PARTICLES_H