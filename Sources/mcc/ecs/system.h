#ifndef MCC_SYSTEM_H
#define MCC_SYSTEM_H

#include <set>
#include "mcc/ecs/entity.h"
#include "mcc/engine/tick.h"

namespace mcc {
  class System {
    friend class Systems;
    friend class SystemManager;
  protected:
    std::set<Entity> entities_;
  public:
    System() = default;
    virtual ~System() = default;
  };

  class TickSystem : public System, public TickListener  {
  public:
    TickSystem() = default;
    ~TickSystem() override = default;
  };

  class Systems {
    DEFINE_NON_INSTANTIABLE_TYPE(Systems);
  private:
    static void SetSignature(const char* type, const Signature& sig);
    static void Register(const char* type, System* sys);

    template<typename T>
    static inline const char*
    TypeId() {
      return typeid(T).name();
    }
  public:
    template<typename T>
    static inline T*
    Register() {
      const auto system = new T();
      Register(TypeId<T>(), system);
      return system;
    }

    template<typename T>
    static inline void
    SetSignature(Signature sig) {
      return SetSignature(TypeId<T>(), sig);
    }

    static void OnDestroyed(const Entity e);
    static void OnSignatureChanged(const Entity e, Signature sig);
  };
}

#endif //MCC_SYSTEM_H