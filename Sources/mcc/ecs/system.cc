#include "mcc/ecs/system.h"
#include "mcc/engine/engine.h"

namespace mcc {
  struct System {
    EntitySet entities;
    Signature signature;
  };

  static std::unordered_map<const char*, System*> systems_;

  void Systems::Register(const char* typeName) {
    systems_.insert({ typeName, new System() });
  }

  void Systems::SetSignature(const char* typeName, const Signature& sig) {
    const auto pos = systems_.find(typeName);
    if(pos == systems_.end())
      return;
    pos->second->signature = sig;
  }

  void Systems::OnDestroyed(const Entity e) {
    for(const auto& pair : systems_) {
      const auto& system = pair.second;
      system->entities.erase(e);
    }
  }

  void Systems::OnSignatureChanged(const Entity e, Signature sig) {
    for(const auto& pair : systems_) {
      const auto& system = pair.second;
      if((sig & system->signature) == system->signature) {
        system->entities.insert(e);
      } else {
        system->entities.erase(e);
      }
    }
  }

  void Systems::ForEachEntityInSystem(const char* typeName, EntityCallback callback) {
    const auto pos = systems_.find(typeName);
    if(pos == systems_.end())
      return;
    const auto& entities = pos->second->entities;
    std::for_each(entities.begin(), entities.end(), callback);
  }
}