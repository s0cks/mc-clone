#include <vector>

#include "mcc/vao.h"
#include "mcc/particle/particles.h"
#include "mcc/engine/engine.h"
#include "mcc/shader/shader.h"

namespace mcc::particle {
  static VertexArrayObject kParticleVao(kInvalidVertexArrayObject);
  static ShaderRef kParticleShader;

  static constexpr const auto kMaxNumberOfParticles = 500;
  static std::vector<Particle> particles_;

  static uint32_t last_used_ = 0;

  static inline uint32_t GetFirstUnusedParticleIndex() {
    for(auto i = last_used_; i < kMaxNumberOfParticles; i++) {
      if(particles_[i].age <= 0) {
        last_used_ = i;
        return i;
      }
    }

    for(auto i = 0; i < last_used_; i++) {
      if(particles_[i].age <= 0) {
        last_used_ = i;
        return i;
      }
    }

    last_used_ = 0;
    return 0;
  }

  static const float kParticleVertices[] = {
    -0.5f, 0.5f, -0.5f, -0.5f, 
     0.5f, 0.5f,  0.5f, -0.5f,
  };

  void Particles::OnPreInit() {

  }

  void Particles::OnInit() {

  }

  void Particles::OnPostInit() {
    particles_.reserve(kMaxNumberOfParticles);
  }

  void Particles::Init() {
    Engine::OnPreInit(&OnPreInit);
    Engine::OnInit(&OnInit);
    Engine::OnPostInit(&OnPostInit);
  }

  void Particles::UpdateAll(const Tick& tick) {
    for(auto idx = 0; idx < kMaxNumberOfParticles; idx++) {
      auto& p = particles_[idx];
      if(p.age > 0) {
        const auto velocity = (p.speed * ((1.0f * tick.dts) / NSEC_PER_SEC));
        p.position += (p.direction * velocity);
      }
    }
  }

  void Particles::RenderAll() {

  }
}