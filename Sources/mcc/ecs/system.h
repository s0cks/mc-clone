#ifndef MCC_SYSTEM_H
#define MCC_SYSTEM_H

#include <set>
#include <functional>
#include "mcc/ecs/entity.h"
#include "mcc/engine/tick.h"

namespace mcc {
  class Systems {
    DEFINE_NON_INSTANTIABLE_TYPE(Systems);
  private:
    static void SetSignature(const char* type, const Signature& sig);
    static void Register(const char* type);
    static void ForEachEntityInSystem(const char* type, EntityCallback callback);

    template<typename T>
    static inline const char*
    TypeId() {
      return typeid(T).name();
    }
  public:
    template<typename T>
    static inline void
    Register() {
      return Register(TypeId<T>());
    }

    template<typename T>
    static inline void
    SetSignature(Signature sig) {
      return SetSignature(TypeId<T>(), sig);
    }

    template<typename T>
    static inline void
    ForEachEntityInSystem(EntityCallback callback) {
      return ForEachEntityInSystem(TypeId<T>(), callback);
    }

    static void Init();
    static void OnDestroyed(const Entity& e);
    static void OnSignatureChanged(const Entity::SignatureChangedEvent& e);
  };
}

#endif //MCC_SYSTEM_H