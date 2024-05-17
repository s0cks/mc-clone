#ifndef MCC_TEXT_EVENTS_H
#define MCC_TEXT_EVENTS_H

#include "mcc/event.h"
#include "mcc/common.h"

namespace mcc::text {
#define FOR_EACH_TEXT_EVENT(V)        \
  V(TextCreated)                      \
  V(TextDestroyed)

  class TextEvent;
#define FORWARD_DECLARE(Name) class Name##Event;
  FOR_EACH_TEXT_EVENT(FORWARD_DECLARE)
#undef FORWARD_DECLARE

  rx::observable<TextEvent*> OnEvent();

  class Text;
  class TextEvent : public Event {
  protected:
    Text* text_;

    explicit TextEvent(Text* text):
      Event(),
      text_(text) {
      MCC_ASSERT(text);
    }
  public:
    ~TextEvent() override = default;

    Text* GetText() const {
      return text_;
    }

    DEFINE_EVENT_PROTOTYPE(FOR_EACH_TEXT_EVENT);
  };

#define DECLARE_TEXT_EVENT(Name)          \
  DECLARE_EVENT_TYPE(TextEvent, Name);

  class TextCreatedEvent : public TextEvent {
  public:
    explicit TextCreatedEvent(Text* text):
      TextEvent(text) {
    }
    ~TextCreatedEvent() override = default;
    DECLARE_TEXT_EVENT(TextCreated);
  };

  class TextDestroyedEvent : public TextEvent {
  public:
    explicit TextDestroyedEvent(Text* text):
      TextEvent(text) {  
    }
    ~TextDestroyedEvent() override = default;
    DECLARE_TEXT_EVENT(TextDestroyed);
  };

#define DEFINE_ON_TEXT_EVENT(Name)                  \
  static inline rx::observable<Name##Event*>        \
  On##Name##Event() {                               \
    return OnEvent()                                \
      .filter(Name##Event::Filter)                  \
      .map(Name##Event::Cast);                      \
  }
  FOR_EACH_TEXT_EVENT(DEFINE_ON_TEXT_EVENT);
#undef DEFINE_ON_TEXT_EVENT
}

#undef DECLARE_TEXT_EVENT

#endif //MCC_TEXT_EVENTS_H