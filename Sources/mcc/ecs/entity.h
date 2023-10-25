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
#include "mcc/event/event_bus.h"

namespace mcc {
  typedef uint64_t EntityId;
  static constexpr const EntityId kInvalidEntityId = 0;

  class Entity {
  public:
    struct SignatureChangedEvent {
      const EntityId id;
      Signature signature;
    };
  private:
    EntityId id_;
  public:
    constexpr Entity(const EntityId id = kInvalidEntityId):
      id_(id) {
    }
    constexpr Entity(const Entity& rhs):
      id_(rhs.id_) {
    }
    ~Entity() = default;

    EntityId id() const {
      return id_;
    }

    constexpr operator EntityId() const {
      return id_;
    }

    void operator=(const Entity& rhs) {
      id_ = rhs.id_;
    }

    void operator=(const EntityId rhs) {
      id_ = rhs;
    }
  public:
    static void OnDestroyed(std::function<void(const Entity&)> callback);
    static void OnSignatureChanged(std::function<void(const SignatureChangedEvent&)> callback);
  };

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