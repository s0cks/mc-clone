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

#define DEFINE_TYPE_CHECK(Name)                                           \
    virtual Name##Event* As##Name##Event() { return nullptr; }            \
    bool Is##Name##Event() { return As##Name##Event() != nullptr; }
    FOR_EACH_VBO_EVENT(DEFINE_TYPE_CHECK)
#undef DEFINE_TYPE_CHECK
  };

#define DECLARE_VBO_EVENT(Name)                                     \
  public:                                                           \
    std::string ToString() const override;                          \
    Name##Event* As##Name##Event() override { return this; }        \
    const char* GetName() const override { return #Name; }          \
    static inline Name##Event*                                      \
    Cast(VboEvent* event) {                                         \
      MCC_ASSERT(event);                                            \
      MCC_ASSERT(event->Is##Name##Event());                         \
      return event->As##Name##Event();                              \
    }                                                               \
    static inline bool                                              \
    Filter(VboEvent* event) {                                       \
      return event                                                  \
          && event->Is##Name##Event();                              \
    }                                                               \
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