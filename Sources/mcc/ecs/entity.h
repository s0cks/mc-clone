#ifndef MCC_ENTITY_H
#define MCC_ENTITY_H

#include <cstdint>
#include <unordered_set>
#include <unordered_map>
#include <iostream>
#include <functional>
#include <array>
#include <queue>
#include <bitset>
#include <set>
#include <glog/logging.h>

#include "mcc/rx.h"
#include "mcc/common.h"

namespace mcc {
  typedef uint64_t EntityId;
  static constexpr const EntityId kInvalidEntityId = 0;
  typedef std::bitset<32> Signature;

#define FOR_EACH_ENTITY_EVENT(V) \
  V(Created)                     \
  V(Destroyed)                   \
  V(SignatureChanged)

#define FORWARD_DECLARE(Name) \
  struct Entity##Name##Event;
  FOR_EACH_ENTITY_EVENT(FORWARD_DECLARE)
#undef FORWARD_DECLARE

  class Entity {
  public:
    struct HashFunction {
      size_t operator()(const Entity& k) const {
        return k.id();
      }
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

    void operator=(const EntityId& rhs) {
      id_ = rhs;
    }

    bool operator==(const Entity& rhs) {
      return id_ == rhs.id_;
    }

    bool operator!=(const Entity& rhs) {
      return id_ != rhs.id_;
    }

    friend std::ostream& operator<<(std::ostream& stream, const Entity& rhs) {
      stream << "Entity(";
      stream << "id=" << rhs.id_;
      stream << ")";
      return stream;
    }
  public:
#define DECLARE_GET_EVENT_OBSERVABLE(Name) \
    static rx::observable<Entity##Name##Event*> On##Name();
    FOR_EACH_ENTITY_EVENT(DECLARE_GET_EVENT_OBSERVABLE);
#undef DECLARE_GET_EVENT_OBSERVABLE
  };

  struct EntitySignatureChangedEvent {
    const EntityId id;
    Signature signature;

    friend std::ostream& operator<<(std::ostream& stream, const EntitySignatureChangedEvent& rhs) {
      stream << "EntitySignatureChangedEvent(";
      stream << "id=" << rhs.id << ", ";
      stream << "signature=" << rhs.signature;
      stream << ")";
      return stream;
    }
  };

  struct EntityDestroyedEvent {
    const EntityId id;

    friend std::ostream& operator<<(std::ostream& stream, const EntityDestroyedEvent& rhs) {
      stream << "EntityDestroyedEvent(";
      stream << "id=" << rhs.id;
      stream << ")";
      return stream;
    }
  };

  static constexpr const uint64_t kMaxNumberOfEntities = 32;

  class Entities {
    DEFINE_NON_INSTANTIABLE_TYPE(Entities);
  public:
    static void Initialize();
    static Entity CreateEntity();
    static void SetSignature(const Entity e, const Signature& sig);
    static Signature GetSignature(const Entity e);
  };

  typedef std::unordered_set<Entity, Entity::HashFunction> EntitySet;
  typedef std::function<void(const Entity)> EntityCallback;
}

#endif //MCC_ENTITY_H