#ifndef MCC_VBO_EVENTS_H
#define MCC_VBO_EVENTS_H

#include "mcc/event.h"
#include "mcc/vbo/vbo_id.h"

namespace mcc::vbo {
#define FOR_EACH_VBO_EVENT(V)    \
  V(VboCreated)                  \
  V(VboDestroyed)

  class Vbo;
  class VboEvent;
#define FORWARD_DECLARE(Name) class Name##Event;
  FOR_EACH_VBO_EVENT(FORWARD_DECLARE)
#undef FORWARD_DECLARE

  class VboEvent : public Event {
  protected:
    const Vbo* vbo_;

    explicit VboEvent(const Vbo* vbo):
      Event(),
      vbo_(vbo) {
    }
  public:
    ~VboEvent() override = default;

    const Vbo* GetVbo() const {
      return vbo_;
    }

    VboId GetVboId() const;
    DEFINE_EVENT_PROTOTYPE(FOR_EACH_VBO_EVENT);
  };

#define DECLARE_VBO_EVENT(Name)                                     \
    DECLARE_EVENT_TYPE(VboEvent, Name)                              \
    static inline std::function<bool(VboEvent*)>                    \
    FilterBy(const VboId id) {                                      \
      return [id](VboEvent* event) {                                \
        return event                                                \
            && event->Is##Name##Event()                             \
            && event->GetVboId() == id;                             \
      };                                                            \
    }

  class VboCreatedEvent : public VboEvent {
  public:
    explicit VboCreatedEvent(const Vbo* vbo):
      VboEvent(vbo) {
    }
    ~VboCreatedEvent() override = default;
    DECLARE_VBO_EVENT(VboCreated);
  };

  class VboDestroyedEvent : public VboEvent {
  public:
    explicit VboDestroyedEvent(const Vbo* vbo):
      VboEvent(vbo) {
    }
    ~VboDestroyedEvent() override = default;
    DECLARE_VBO_EVENT(VboDestroyed);
  };
}

#endif //MCC_VBO_EVENTS_H