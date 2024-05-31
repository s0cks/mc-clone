#ifndef MCC_CURSOR_EVENTS_H
#define MCC_CURSOR_EVENTS_H

#include "mcc/event.h"
#include "mcc/common.h"

namespace mcc::mouse {
#define FOR_EACH_CURSOR_EVENT(V)          \
  V(CursorCreated)                        \
  V(CursorDeleted)                        \
  V(CursorChanged)

  class Cursor;
  class CursorEvent;
#define FORWARD_DECLARE(Name) class Name##Event;
  FOR_EACH_CURSOR_EVENT(FORWARD_DECLARE)
#undef FORWARD_DECLARE

  class CursorEvent : public Event {
  protected:
    const Cursor* cursor_;

    explicit CursorEvent(const Cursor* cursor):
      Event(),
      cursor_(cursor) {
      MCC_ASSERT(cursor);
    }
  public:
    ~CursorEvent() override = default;

    const Cursor* GetCursor() const {
      return cursor_;
    }

    DEFINE_EVENT_PROTOTYPE(FOR_EACH_CURSOR_EVENT);
  };

#define DECLARE_CURSOR_EVENT(Name)        \
  DECLARE_EVENT_TYPE(CursorEvent, Name);

  class CursorCreatedEvent : public CursorEvent {
  public:
    explicit CursorCreatedEvent(const Cursor* cursor):
      CursorEvent(cursor) {
    }
    ~CursorCreatedEvent() override = default;
    DECLARE_CURSOR_EVENT(CursorCreated);
  };

  class CursorDeletedEvent : public CursorEvent {
  public:
    explicit CursorDeletedEvent(const Cursor* cursor):
      CursorEvent(cursor) {  
    }
    ~CursorDeletedEvent() override = default;
    DECLARE_CURSOR_EVENT(CursorDeleted);
  };
  
  class CursorChangedEvent : public CursorEvent {
  public:
    explicit CursorChangedEvent(const Cursor* cursor):
      CursorEvent(cursor) {
    }
    ~CursorChangedEvent() override = default;
    DECLARE_CURSOR_EVENT(CursorChanged);
  };
}

#endif //MCC_CURSOR_EVENTS_H