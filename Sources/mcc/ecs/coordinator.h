#ifndef MCC_COORDINATOR_H
#define MCC_COORDINATOR_H

#include "mcc/ecs/component.h"
#include "mcc/ecs/entity.h"
#include "mcc/fixed_rate_loop.h"

namespace mcc {
  class Coordinator {
    DEFINE_NON_INSTANTIABLE_TYPE(Coordinator);
  public:
    template<typename T>
    static inline ComponentState<T> AddComponent(const Entity e, const T& component) {
      auto state = ComponentState<T>(component);
      if(!T::PutState(e, state)) {
        LOG(ERROR) << "failed to put " << state << " for " << e;
        return state;
      }

      auto sig = Entities::GetSignature(e);
      sig.set(Components::GetComponentIdForType<T>(), true);
      Entities::SetSignature(e, sig);
      return state;
    }

    template<typename T>
    static inline void RemoveComponent(const Entity e) {
      LOG_IF(ERROR, !T::RemoveState(e)) << "failed to remove state for " << e;
      auto sig = Entities::GetSignature(e);
      sig.set(Components::GetComponentIdForType<T>(), false);
      Entities::SetSignature(e, sig);
    }
  };
}

#endif //MCC_COORDINATOR_H