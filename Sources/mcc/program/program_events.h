#ifndef MCC_PROGRAM_EVENTS_H
#define MCC_PROGRAM_EVENTS_H

#include "mcc/event.h"
#include "mcc/common.h"
#include "mcc/shader/shader_id.h"
#include "mcc/program/program_id.h"

namespace mcc::program {
#define FOR_EACH_PROGRAM_EVENT(V) \
  V(ProgramCreated)               \
  V(ProgramShaderAttached)        \
  V(ProgramShaderDetached)        \
  V(ProgramLinked)                \
  V(ProgramDestroyed)

  class ProgramEvent;
#define FORWARD_DECLARE(Name) class Name##Event;
  FOR_EACH_PROGRAM_EVENT(FORWARD_DECLARE)
#undef FORWARD_DECLARE

  class ProgramEvent : public Event {
  protected:
    ProgramId id_;

    explicit ProgramEvent(const ProgramId id):
      Event(),
      id_(id) {
    }
  public:
    ~ProgramEvent() override = default;

    ProgramId GetProgramId() const {
      return id_;
    }

#define DEFINE_TYPE_CHECK(Name)                                               \
    virtual Name##Event* As##Name##Event() { return nullptr; }                \
    bool Is##Name##Event() { return As##Name##Event() != nullptr; }
    FOR_EACH_PROGRAM_EVENT(DEFINE_TYPE_CHECK)
#undef DEFINE_TYPE_CHECK
  };

#define DEFINE_PROGRAM_EVENT(Name) \
  public:                                                                     \
    const char* GetName() const override { return #Name; }                    \
    std::string ToString() const override;                                    \
    Name##Event* As##Name##Event() override { return this; }                  \
    static inline Name##Event*                                                \
    Cast(ProgramEvent* event) {                                               \
      MCC_ASSERT(event);                                                      \
      MCC_ASSERT(event->Is##Name##Event());                                   \
      return event->As##Name##Event();                                        \
    }                                                                         \
    static inline bool                                                        \
    Filter(ProgramEvent* event) {                                             \
      return event                                                            \
          && event->Is##Name##Event();                                        \
    }                                                                         \
    static inline std::function<bool(ProgramEvent*)>                          \
    FilterBy(const ProgramId id) {                                            \
      return [id](ProgramEvent* event) {                                      \
        return event                                                          \
            && event->Is##Name##Event()                                       \
            && event->GetProgramId() == id;                                   \
      };                                                                      \
    }

  class ProgramCreatedEvent : public ProgramEvent {
  public:
    explicit ProgramCreatedEvent(const ProgramId id):
      ProgramEvent(id) {
    }
    ~ProgramCreatedEvent() override = default;
    DEFINE_PROGRAM_EVENT(ProgramCreated);
  };

  class ProgramShaderEvent : public ProgramEvent {
  protected:
    ShaderId shader_;
  public:
    explicit ProgramShaderEvent(const ProgramId id, const ShaderId shader):
      ProgramEvent(id),
      shader_(shader) {
    }
    ~ProgramShaderEvent() override = default;

    ShaderId GetShaderId() const {
      return shader_;
    }
  };

  class ProgramShaderAttachedEvent : public ProgramShaderEvent {
  public:
    ProgramShaderAttachedEvent(const ProgramId id, const ShaderId shader):
      ProgramShaderEvent(id, shader) {
    }
    ~ProgramShaderAttachedEvent() override = default;
    DEFINE_PROGRAM_EVENT(ProgramShaderAttached);
  };

  class ProgramShaderDetachedEvent : public ProgramShaderEvent {
  public:
    ProgramShaderDetachedEvent(const ProgramId id, const ShaderId shader):
      ProgramShaderEvent(id, shader) {
    }
    ~ProgramShaderDetachedEvent() override = default;
    DEFINE_PROGRAM_EVENT(ProgramShaderDetached);
  };

  class ProgramLinkedEvent : public ProgramEvent {
  public:
    explicit ProgramLinkedEvent(const ProgramId id):
      ProgramEvent(id) {
    }
    ~ProgramLinkedEvent() override = default;
    DEFINE_PROGRAM_EVENT(ProgramLinked);
  };

  class ProgramDestroyedEvent : public ProgramEvent {
  public:
    explicit ProgramDestroyedEvent(const ProgramId id):
      ProgramEvent(id) {
    }
    ~ProgramDestroyedEvent() override = default;
    DEFINE_PROGRAM_EVENT(ProgramDestroyed);
  };

#undef DEFINE_PROGRAM_EVENT
}

#endif //MCC_PROGRAM_EVENTS_H