#include "mcc/ecs/system.h"

namespace mcc {
  static std::unordered_map<const char*, Signature> signatures_;
  static std::unordered_map<const char*, System*> systems_;

  void Systems::Register(const char* typeName, System* system) {
    systems_.insert({ typeName, system });
  }

  void Systems::SetSignature(const char* typeName, const Signature& sig) {
    signatures_.insert({ typeName, sig });
  }

  void Systems::OnDestroyed(const Entity e) {
    for(const auto& pair : systems_) {
      const auto& system = pair.second;
      system->entities_.erase(e);
    }
  }

  void Systems::OnSignatureChanged(const Entity e, Signature sig) {
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
}