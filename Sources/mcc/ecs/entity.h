#ifndef MCC_ENTITY_H
#define MCC_ENTITY_H

#include <cstdint>
#include <unordered_set>
#include <unordered_map>
#include <functional>
#include <array>
#include <queue>
#include <bitset>
#include <set>

#include "mcc/common.h"

namespace mcc {
  typedef uint64_t Entity;

  typedef std::bitset<32> Signature;

  static constexpr const uint64_t kMaxNumberOfEntities = 32;

  class Entities {
    DEFINE_NON_INSTANTIABLE_TYPE(Entities);
  public:
    static void Initialize();
    static Entity CreateEntity();
    static void OnDestroyed(const Entity e);
    static void SetSignature(const Entity e, const Signature& sig);
    static Signature GetSignature(const Entity e);
  };

  typedef std::unordered_set<Entity> EntitySet;
  typedef std::function<void(const Entity)> EntityCallback;
}

#endif //MCC_ENTITY_H