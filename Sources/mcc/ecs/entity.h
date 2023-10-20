#ifndef MCC_ENTITY_H
#define MCC_ENTITY_H

#include <cstdint>
#include <unordered_map>
#include <array>
#include <queue>
#include <bitset>
#include <set>

#include "mcc/common.h"

namespace mcc {
  typedef uint64_t Entity;

  typedef std::bitset<32> Signature;

  static constexpr const uint64_t kMaxNumberOfEntities = 32;

  class Coordinator;
  class EntityManager {
    friend class Coordinator;
  protected:
    std::queue<Entity> available_;
    std::array<Signature, kMaxNumberOfEntities> signatures_;
    uint32_t living_;

    EntityManager():
      available_(),
      signatures_(),
      living_(0) {
      for(auto e = 0; e < kMaxNumberOfEntities; e++)
        available_.push(e);
    }
  public:
    virtual ~EntityManager() = default;
    DEFINE_NON_COPYABLE_TYPE(EntityManager);

    Entity CreateEntity() {
      const auto id = available_.front();
      available_.pop();
      living_++;
      return id;
    }

    void OnDestroyed(const Entity e) {
      signatures_[e].reset();
      available_.push(e);
      living_--;
    }

    void SetSignature(const Entity e, const Signature& sig) {
      signatures_[e] = sig;
    }

    Signature GetSignature(const Entity e) {
      return signatures_[e];
    }
  };
}

#endif //MCC_ENTITY_H