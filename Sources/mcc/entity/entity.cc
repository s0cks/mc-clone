#include "mcc/entity/entity.h"

namespace mcc {
  static std::queue<Entity> available_;
  static std::array<Signature, kMaxNumberOfEntities> signatures_;
  static uint32_t living_;

#define DEFINE_EVENT_SUBJECT(Name) \
  static const rxsub::subject<Entity##Name##Event*> on##Name##Event_;
  FOR_EACH_ENTITY_EVENT(DEFINE_EVENT_SUBJECT)
#undef DEFINE_EVENT_SUBJECT

  void Entity::UpdateSignature(const ComponentId id, const bool value) const {
    auto sig = Entities::GetSignature(*this);
    sig.set(id, value);
    Entities::SetSignature((*this), sig);
  }

  void Entities::Initialize() {
    for(auto e = 0; e < kMaxNumberOfEntities; e++)
        available_.push(e);
    Entity::OnDestroyed()
      .subscribe([&](EntityDestroyedEvent* e) {
        signatures_[e->id].reset();
        available_.push(e->id);
        living_--;
      });
  }

  Entity Entities::CreateEntity() {
    const auto id = available_.front();
    available_.pop();
    living_++;
    return id;
  }

  void Entities::SetSignature(const Entity e, const Signature& sig) {
    signatures_[e] = sig;
    auto event = EntitySignatureChangedEvent {
      .id = (EntityId)e,
      .signature = sig,
    };
    onSignatureChangedEvent_.get_subscriber().on_next(&event);
  }

  Signature Entities::GetSignature(const Entity e) {
    return signatures_[e];
  }

#define DEFINE_GET_EVENT_OBSERVABLE(Name)                   \
  rx::observable<Entity##Name##Event*> Entity::On##Name() { \
    return on##Name##Event_.get_observable();               \
  }
  FOR_EACH_ENTITY_EVENT(DEFINE_GET_EVENT_OBSERVABLE)
#undef DEFINE_GET_EVENT_OBSERVABLE
}