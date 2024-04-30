#ifndef MCC_CAMERA_EVENTS_H
#define MCC_CAMERA_EVENTS_H

#include "mcc/event.h"
#include "mcc/common.h"

namespace mcc::camera {
#define FOR_EACH_CAMERA_EVENT(V)    \
  V(CameraCreated)                  \
  V(CameraDestroyed)

  class Camera;
  class CameraEvent;
#define FORWARD_DECLARE(Name) class Name##Event;
  FOR_EACH_CAMERA_EVENT(FORWARD_DECLARE)
#undef FORWARD_DECLARE

  class CameraEvent : public Event {
  protected:
    CameraEvent() = default;
  public:
    ~CameraEvent() override = default;

#define DEFINE_TYPE_CHECK(Name)                                               \
    virtual Name##Event* As##Name##Event() { return nullptr; }                \
    bool Is##Name##Event() { return As##Name##Event() != nullptr; }
    FOR_EACH_CAMERA_EVENT(DEFINE_TYPE_CHECK)
#undef DEFINE_TYPE_CHECK
  };

#define DECLARE_CAMERA_EVENT(Name)                                            \
  public:                                                                     \
    std::string ToString() const override;                                    \
    const char* GetName() const override { return #Name; }                    \
    Name##Event* As##Name##Event() override { return this; }                  \
    static inline bool                                                        \
    Filter(CameraEvent* event) {                                              \
      return event                                                            \
          && event->Is##Name##Event();                                        \
    }                                                                         \
    static inline Name##Event*                                                \
    Cast(CameraEvent* event) {                                                \
      MCC_ASSERT(event);                                                      \
      MCC_ASSERT(event->Is##Name##Event());                                   \
      return event->As##Name##Event();                                        \
    }

  class CameraCreatedEvent : public CameraEvent {
  public:
    CameraCreatedEvent() = default;
    ~CameraCreatedEvent() override = default;
    DECLARE_CAMERA_EVENT(CameraCreated);
  };
  
  class CameraDestroyedEvent : public CameraEvent {
  public:
    CameraDestroyedEvent() = default;
    ~CameraDestroyedEvent() override = default;
    DECLARE_CAMERA_EVENT(CameraDestroyed);
  };
}

#endif //MCC_CAMERA_EVENTS_H