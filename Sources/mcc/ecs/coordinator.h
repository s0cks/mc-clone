#ifndef MCC_COORDINATOR_H
#define MCC_COORDINATOR_H

#include "mcc/ecs/component.h"
#include "mcc/ecs/system.h"
#include "mcc/ecs/entity.h"
#include "mcc/fixed_rate_loop.h"

namespace mcc {
  class Coordinator {
    DEFINE_NON_INSTANTIABLE_TYPE(Coordinator);
  public:
    template<typename T>
    static inline ComponentState<T> AddComponent(const Entity e, const T& component) {
      const auto state = ComponentState<T>(component);
      Components::AddComponent<T>(e, component);
      auto sig = Entities::GetSignature(e);
      sig.set(Components::GetComponentIdForType<T>(), true);
      Entities::SetSignature(e, sig);
      return state;
    }

    template<typename T>
    static inline void RemoveComponent(const Entity e) {
      Components::RemoveComponent<T>(e);
      auto sig = Entities::GetSignature(e);
      sig.set(Components::GetComponentIdForType<T>(), false);
      Entities::SetSignature(e, sig);
    }
  };
}

#endif //MCC_COORDINATOR_H