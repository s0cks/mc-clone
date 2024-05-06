#ifndef MCC_SKYBOX_EVENTS_H
#define MCC_SKYBOX_EVENTS_H

#include "mcc/event.h"
#include "mcc/common.h"

namespace mcc::skybox {
#define FOR_EACH_SKYBOX_EVENT(V)      \
  V(SkyboxCreated)                    \
  V(SkyboxDestroyed)                  \
  V(SkyboxTextureChanged)             \
  V(SkyboxChanged)

  class Skybox;
  class SkyboxEvent;
#define FORWARD_DECLARE(Name) class Name##Event;
  FOR_EACH_SKYBOX_EVENT(FORWARD_DECLARE)
#undef FORWARD_DECLARE

  class SkyboxEvent : public Event {
  protected:
    const Skybox* skybox_;

    explicit SkyboxEvent(const Skybox* skybox):
      Event(),
      skybox_(skybox) {
    }
  public:
    ~SkyboxEvent() override = default;

    const Skybox* GetSkybox() const {
      return skybox_;
    }
    DEFINE_EVENT_PROTOTYPE(FOR_EACH_SKYBOX_EVENT);
  };

#define DECLARE_SKYBOX_EVENT(Name)        \
  DECLARE_EVENT_TYPE(SkyboxEvent, Name)

  class SkyboxCreatedEvent : public SkyboxEvent {
  public:
    explicit SkyboxCreatedEvent(const Skybox* skybox):
      SkyboxEvent(skybox) {
    }
    ~SkyboxCreatedEvent() override = default;
    DECLARE_SKYBOX_EVENT(SkyboxCreated);
  };

  class SkyboxDestroyedEvent : public SkyboxEvent {
  public:
    explicit SkyboxDestroyedEvent(const Skybox* skybox):
      SkyboxEvent(skybox) {
    }
    ~SkyboxDestroyedEvent() override = default;
    DECLARE_SKYBOX_EVENT(SkyboxDestroyed);
  };

  class SkyboxTextureChangedEvent : public SkyboxEvent {
  public:
    SkyboxTextureChangedEvent(const Skybox* skybox):
      SkyboxEvent(skybox) {
    }
    ~SkyboxTextureChangedEvent() override = default;
    DECLARE_SKYBOX_EVENT(SkyboxTextureChanged);
  };

  class SkyboxChangedEvent : public SkyboxEvent {
  protected:
    const Skybox* previous_;
  public:
    explicit SkyboxChangedEvent(const Skybox* skybox,
                                const Skybox* previous = nullptr):
      SkyboxEvent(skybox),
      previous_(previous) {
    }
    ~SkyboxChangedEvent() override = default;

    const Skybox* GetPrevious() const {
      return previous_;
    }

    bool HasPrevious() const {
      return previous_ != nullptr;
    }

    DECLARE_SKYBOX_EVENT(SkyboxChanged);
  };
}

#endif //MCC_SKYBOX_EVENTS_H