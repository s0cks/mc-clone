#ifndef MCC_PARTICLE_H
#define MCC_PARTICLE_H

#include "mcc/gfx.h"
#include "mcc/platform.h"

namespace mcc::particle {
  typedef uint64_t ParticleDuration;

  struct Particle {
    glm::vec3 position;
    glm::vec3 direction;
    float speed;
    glm::vec3 color;
    ParticleDuration age;
    ParticleDuration max_age;
  };
}

#endif //MCC_PARTICLE_H