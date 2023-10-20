#ifndef MCC_SYSTEM_H
#define MCC_SYSTEM_H

#include <set>
#include "mcc/ecs/entity.h"

namespace mcc {
  class SystemManager;
  class System {
    friend class SystemManager;
  protected:
    std::set<Entity> entities_;
  public:
    System() = default;
    virtual ~System() = default;
  };

  class TickSystem : public System {
  public:
    TickSystem() = default;
    ~TickSystem() override = default;
    virtual void Update(const uint64_t dts) = 0;
  };

  class Coordinator;
  class SystemManager {
    friend class Coordinator;
  protected:
    std::unordered_map<const char*, Signature> signatures_;
    std::unordered_map<const char*, System*> systems_;

    SystemManager() = default;
  public:
    virtual ~SystemManager() = default;

    template<typename T>
    T* RegisterSystem() {
      const char* typeName = typeid(T).name();
      const auto system = new T();
      systems_.insert({ typeName, system });
      return system;
    }

    template<typename T>
    void SetSignature(Signature sig) {
      const char* typeName = typeid(T).name();
      signatures_.insert({ typeName, sig });
    }

    void OnDestroyed(const Entity e) {
      for(const auto& pair : systems_) {
        const auto& system = pair.second;
        system->entities_.erase(e);
      }
    }

    void OnSignatureChanged(const Entity e, Signature sig) {
      for(const auto& pair : systems_) {
        const auto& type = pair.first;
        const auto& system = pair.second;
        const auto& sysSig = signatures_[type];
        if((sig & sysSig) == sysSig) {
          system->entities_.insert(e);
        } else {
          system->entities_.erase(e);
        }
      }
    }
  };
}

#endif //MCC_SYSTEM_H