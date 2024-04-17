#ifndef MCC_TEXTURE_EVENT_H
#define MCC_TEXTURE_EVENT_H

#include "mcc/event.h"
#include "mcc/texture/texture_constants.h"

namespace mcc::texture {
#define FOR_EACH_TEXTURE_EVENT(V) \
  V(TextureCreated)               \
  V(TextureDestroyed)

  class TextureEvent;
#define FORWARD_DECLARE(Name) class Name##Event;
  FOR_EACH_TEXTURE_EVENT(FORWARD_DECLARE)
#undef FORWARD_DECLARE

#define DECLARE_TEXTURE_EVENT(Name)                               \
  public:                                                         \
    const char* GetName() const override { return #Name; }        \
    std::string ToString() const override;                        \
    Name##Event* As##Name##Event() override { return this; }      \
    static inline bool                                            \
    FilterBy(TextureEvent* event) {                               \
      MCC_ASSERT(event);                                          \
      return event->Is##Name##Event();                            \
    }                                                             \
    static inline Name##Event*                                    \
    Cast(TextureEvent* event) {                                   \
      MCC_ASSERT(event);                                          \
      MCC_ASSERT(event->Is##Name##Event());                       \
      return event->As##Name##Event();                            \
    }

  class TextureEvent : public Event {
  protected:
    TextureId id_;

    explicit TextureEvent(const TextureId id):
      Event(),
      id_(id) {  
    }
  public:
    ~TextureEvent() override = default;

#define DEFINE_TYPE_CHECK(Name)                                       \
    virtual Name##Event* As##Name##Event() { return nullptr; }        \
    bool Is##Name##Event() { return As##Name##Event() != nullptr; }
    FOR_EACH_TEXTURE_EVENT(DEFINE_TYPE_CHECK)
#undef DEFINE_TYPE_CHECK
  };

  class TextureCreatedEvent : public TextureEvent {
  protected:
    explicit TextureCreatedEvent(const TextureId id):
      TextureEvent(id) {
    }
  public:
    ~TextureCreatedEvent() override = default;
    DECLARE_TEXTURE_EVENT(TextureCreated);
  public:
    static inline TextureCreatedEvent*
    New(const TextureId id) {
      return new TextureCreatedEvent(id);
    }
  };

  class TextureDestroyedEvent : public TextureEvent {
  protected:
    explicit TextureDestroyedEvent(const TextureId id):
      TextureEvent(id) {
    }
  public:
    ~TextureDestroyedEvent() override = default;
    DECLARE_TEXTURE_EVENT(TextureDestroyed);
  public:
    static inline TextureDestroyedEvent*
    New(const TextureId id) {
      return new TextureDestroyedEvent(id);
    }
  };
}

#endif //MCC_TEXTURE_EVENT_H