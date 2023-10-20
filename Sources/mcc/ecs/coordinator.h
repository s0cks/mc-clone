#ifndef MCC_COORDINATOR_H
#define MCC_COORDINATOR_H

#include "mcc/ecs/component.h"
#include "mcc/ecs/event.h"
#include "mcc/ecs/system.h"
#include "mcc/ecs/entity.h"
#include "mcc/fixed_rate_loop.h"

namespace mcc {
  class Coordinator {
    DEFINE_NON_INSTANTIABLE_TYPE(Coordinator);
  public:
    static inline Entity CreateEntity() {
      return Entities::CreateEntity();
    }

    static inline void DestoryEntity(const Entity e) {
      Entities::OnDestroyed(e);
      Components::OnDestroyed(e);
      Systems::OnDestroyed(e);
    }

    template<typename T>
    static inline void RegisterComponent() {
      Components::Register<T>();
    }

    template<typename T>
    static inline void AddComponent(const Entity e, T component) {
      Components::AddComponent<T>(e, component);
      auto sig = Entities::GetSignature(e);
      sig.set(GetComponentId<T>(), true);
      Entities::SetSignature(e, sig);
      Systems::OnSignatureChanged(e, sig);
    }

    template<typename T>
    static inline void RemoveComponent(const Entity e) {
      Components::RemoveComponent<T>(e);
      auto sig = Entities::GetSignature(e);
      sig.set(GetComponentId<T>(), false);
      Entities::SetSignature(e, sig);
      Systems::OnSignatureChanged(e, sig);
    }

    template<typename T>
    static inline T& GetComponent(const Entity e) {
      return Components::GetComponent<T>(e);
    }

    template<typename T>
    static inline ComponentId GetComponentId() {
      return Components::GetComponentIdForType<T>();
    }
  };
}

#endif //MCC_COORDINATOR_H