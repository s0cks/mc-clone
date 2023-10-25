#include "mcc/ecs/entity.h"

namespace mcc {
  static std::queue<Entity> available_;
  static std::array<Signature, kMaxNumberOfEntities> signatures_;
  static uint32_t living_;
  static EventBus events_;

  static constexpr const char* kSignatureChangedEvent = "Entity::SignatureChanged";
  static constexpr const char* kDestroyedEvent = "Entity::Destroyed";

  void Entity::OnSignatureChanged(std::function<void(const Entity::SignatureChangedEvent&)> callback) {
    events_.Register(kSignatureChangedEvent, callback);
  }

  void Entity::OnDestroyed(std::function<void(const Entity&)> callback) {
    events_.Register(kDestroyedEvent, callback);
  }

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
    const auto event = Entity::SignatureChangedEvent {
      .id = (EntityId)e,
      .signature = sig,
    };
    events_.Publish(kSignatureChangedEvent, event);
  }

  Signature Entities::GetSignature(const Entity e) {
    return signatures_[e];
  }
}