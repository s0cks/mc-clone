#ifndef MCC_ENTITY_EVENTS_H
#define MCC_ENTITY_EVENTS_H

#include "mcc/rx.h"
#include "mcc/event.h"
#include "mcc/common.h"
#include "mcc/entity/entity_signature.h"

namespace mcc::entity {
#define FOR_EACH_ENTITY_EVENT(V) \
  V(EntityCreated)               \
  V(EntitySignatureChanged)      \
  V(EntityDestroyed)

  class EntityEvent;
#define FORWARD_DELARE(Name) class Name##Event;
  FOR_EACH_ENTITY_EVENT(FORWARD_DELARE)
#undef FORWARD_DECLARE

  class EntityEvent : public Event {
  public:
    static inline std::function<bool(EntityEvent*)>
    FilterById(const EntityId id) {
      return [id](EntityEvent* event) {
        MCC_ASSERT(event);
        return event->id() == id;
      };
    }
  protected:
    EntityId id_;

    explicit EntityEvent(const EntityId id):
      Event(),
      id_(id) {
    }
  public:
    ~EntityEvent() override = default;

    EntityId id() const {
      return id_;
    }
    
    DEFINE_EVENT_PROTOTYPE(FOR_EACH_ENTITY_EVENT);
  };

#define DEFINE_ENTITY_EVENT(Name)                                 \
  DECLARE_EVENT_TYPE(EntityEvent, Name)                           \
  static inline std::function<bool(EntityEvent*)>                 \
  FilterBy(const EntityId id) {                                   \
    return [id](EntityEvent* event) {                             \
      return event                                                \
          && event->Is##Name##Event()                             \
          && event->id() == id;                                   \
    };                                                            \
  }

  class EntityCreatedEvent : public EntityEvent {
  public:
    explicit EntityCreatedEvent(const EntityId id):
      EntityEvent(id) {  
    }
    ~EntityCreatedEvent() override = default;
    DEFINE_ENTITY_EVENT(EntityCreated);
  };

  class EntitySignatureChangedEvent : public EntityEvent {
  protected:
    Signature signature_;
  public:
    explicit EntitySignatureChangedEvent(const EntityId id,
                                         const Signature& signature):
      EntityEvent(id),
      signature_(signature) {
    }
    ~EntitySignatureChangedEvent() override = default;
    DEFINE_ENTITY_EVENT(EntitySignatureChanged);

    const Signature& signature() const {
      return signature_;
    }
  };

  class EntityDestroyedEvent : public EntityEvent {
  public:
    explicit EntityDestroyedEvent(const EntityId id):
      EntityEvent(id) {
    }
    ~EntityDestroyedEvent() override = default;
    DEFINE_ENTITY_EVENT(EntityDestroyed);
  };
}

#endif //MCC_ENTITY_EVENTS_H