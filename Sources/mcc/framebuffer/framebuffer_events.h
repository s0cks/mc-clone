#ifndef MCC_FRAMEBUFFER_EVENTS_H
#define MCC_FRAMEBUFFER_EVENTS_H

#include "mcc/event.h"
#include "mcc/framebuffer/framebuffer_constants.h"

namespace mcc::fbuff {
#define FOR_EACH_FRAMEBUFFER_EVENT(V) \
  V(FrameBufferCreated)               \
  V(FrameBufferDestroyed)

  class FrameBufferEvent;
#define FORWARD_DECLARE(Name) class Name##Event;
  FOR_EACH_FRAMEBUFFER_EVENT(FORWARD_DECLARE)
#undef FORWARD_DECLARE

#define DECLARE_FRAMEBUFFER_EVENT(Name)                                 \
  public:                                                               \
    const char* GetName() const override { return #Name; }              \
    std::string ToString() const override;                              \
    static inline bool                                                  \
    FilterBy(FrameBufferEvent* event) {                                 \
      MCC_ASSERT(event);                                                \
      return event->Is##Name##Event();                                  \
    }                                                                   \
    static inline Name##Event*                                          \
    Cast(FrameBufferEvent* event) {                                     \
      MCC_ASSERT(event);                                                \
      MCC_ASSERT(event->Is##Name##Event());                             \
      return event->As##Name##Event();                                  \
    }

  class FrameBufferEvent : public Event {
  protected:
    FrameBufferObjectId id_;

    explicit FrameBufferEvent(const FrameBufferObjectId id):
      Event(),
      id_(id) {
    }
  public:
    ~FrameBufferEvent() override = default;

    FrameBufferObjectId id() const {
      return id_;
    }

#define DEFINE_TYPE_CHECK(Name)                                           \
    virtual Name##Event* As##Name##Event() { return nullptr; }            \
    bool Is##Name##Event() { return As##Name##Event() != nullptr; }
    FOR_EACH_FRAMEBUFFER_EVENT(DEFINE_TYPE_CHECK)
#undef DEFINE_TYPE_CHECK
  };

  class FrameBufferCreatedEvent : public FrameBufferEvent {
  public:
    explicit FrameBufferCreatedEvent(const FrameBufferObjectId id):
      FrameBufferEvent(id) {
    }
    ~FrameBufferCreatedEvent() override = default;
    DECLARE_FRAMEBUFFER_EVENT(FrameBufferCreated);
  };

  class FrameBufferDestroyedEvent : public FrameBufferEvent {
  public:
    explicit FrameBufferDestroyedEvent(const FrameBufferObjectId id):
      FrameBufferEvent(id) {
    }
    ~FrameBufferDestroyedEvent() override = default;
    DECLARE_FRAMEBUFFER_EVENT(FrameBufferDestroyed);
  };
}

#endif //MCC_FRAMEBUFFER_EVENTS_H