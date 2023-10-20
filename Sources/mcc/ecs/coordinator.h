#ifndef MCC_COORDINATOR_H
#define MCC_COORDINATOR_H

#include "mcc/ecs/component.h"
#include "mcc/ecs/event.h"
#include "mcc/ecs/system.h"
#include "mcc/ecs/entity.h"

namespace mcc {
  class Coordinator {
  protected:
    ComponentManager* components_;
    EntityManager* entities_;
    SystemManager* systems_;
    EventManager* events_;

    Coordinator():
      components_(new ComponentManager()),
      entities_(new EntityManager()),
      systems_(new SystemManager()),
      events_(new EventManager()) {
    }

    static void Set(Coordinator* value);
  public:
    virtual ~Coordinator() = default;
    DEFINE_NON_COPYABLE_TYPE(Coordinator);

    Entity CreateEntity() {
      return entities_->CreateEntity();
    }

    void DestoryEntity(const Entity e) {
      entities_->OnDestroyed(e);
      components_->OnDestroyed(e);
      systems_->OnDestroyed(e);
    }

    template<typename T>
    void RegisterComponent() {
      components_->RegisterComponent<T>();
    }

    template<typename T>
    void AddComponent(const Entity e, T component) {
      components_->AddComponent<T>(e, component);
      auto sig = entities_->GetSignature(e);
      sig.set(GetComponentId<T>(), true);
      entities_->SetSignature(e, sig);
      systems_->OnSignatureChanged(e, sig);
    }

    template<typename T>
    void RemoveComponent(const Entity e) {
      components_->RemoveComponent<T>(e);
      auto sig = entities_->GetSignature(e);
      sig.set(GetComponentId<T>(), false);
      entities_->SetSignature(e, sig);
      systems_->OnSignatureChanged(e, sig);
    }

    template<typename T>
    T& GetComponent(const Entity e) {
      return components_->GetComponent<T>(e);
    }

    template<typename T>
    ComponentId GetComponentId() {
      return components_->GetComponentId<T>();
    }

    template<typename T>
    T* RegisterSystem() {
      return systems_->RegisterSystem<T>();
    }

    template<typename T>
    void SetSystemSignature(Signature sig) {
      systems_->SetSignature<T>(sig);
    }

    void Register(const EventId e, EventListener listener) {
      events_->Register(e, listener);
    }

    void Send(Event& e) {
      return events_->Send(e);
    }

    void Send(const EventId id) {
      return events_->Send(id);
    }
  public:
    static Coordinator* Initialize();
    static Coordinator* Get();
  };
}

#endif //MCC_COORDINATOR_H