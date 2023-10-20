#include "mcc/ecs/entity.h"

namespace mcc {
  static std::queue<Entity> available_;
  static std::array<Signature, kMaxNumberOfEntities> signatures_;
  static uint32_t living_;

  void Entities::Initialize() {
    for(auto e = 0; e < kMaxNumberOfEntities; e++)
        available_.push(e);
  }

  void Entities::OnDestroyed(const Entity e) {
    signatures_[e].reset();
    available_.push(e);
    living_--;
  }

  Entity Entities::CreateEntity() {
    const auto id = available_.front();
    available_.pop();
    living_++;
    return id;
  }

  void Entities::SetSignature(const Entity e, const Signature& sig) {
    signatures_[e] = sig;
  }

  Signature Entities::GetSignature(const Entity e) {
    return signatures_[e];
  }
}