#ifndef MCC_MESH_EVENTS_H
#define MCC_MESH_EVENTS_H

#include "mcc/event.h"
#include "mcc/common.h"

namespace mcc::mesh {
#define FOR_EACH_MESH_EVENT(V) \
  V(MeshCreated)               \
  V(MeshDestroyed)

  class Mesh;
  class MeshEvent;
#define FORWARD_DECLARE(Name) class Name##Event;
  FOR_EACH_MESH_EVENT(FORWARD_DECLARE)
#undef FORWARD_DECLARE

  class MeshEvent : public Event {
  protected:
    const Mesh* mesh_;

    explicit MeshEvent(const Mesh* mesh):
      Event(),
      mesh_(mesh) {
    }
  public:
    ~MeshEvent() override = default;

    const Mesh* GetMesh() const {
      return mesh_;
    }

#define DEFINE_TYPE_CHECK(Name)                                       \
    virtual Name##Event* As##Name##Event() { return nullptr; }        \
    bool Is##Name##Event() { return As##Name##Event() != nullptr; }
    FOR_EACH_MESH_EVENT(DEFINE_TYPE_CHECK)
#undef DEFINE_TYPE_CHECK
  };

#define DECLARE_MESH_EVENT(Name)                                                    \
  public:                                                                           \
    std::string ToString() const override;                                          \
    const char* GetName() const override { return #Name; }                          \
    Name##Event* As##Name##Event() override { return this; }                        \
    static inline Name##Event*                                                      \
    Cast(MeshEvent* event) {                                                        \
      MCC_ASSERT(event);                                                            \
      MCC_ASSERT(event->Is##Name##Event());                                         \
      return event->As##Name##Event();                                              \
    }                                                                               \
    static inline bool                                                              \
    Filter(MeshEvent* event) {                                                      \
      return event                                                                  \
        && event->Is##Name##Event();                                                \
    }
  
  class MeshCreatedEvent : public MeshEvent {
  public:
    explicit MeshCreatedEvent(const Mesh* mesh):
      MeshEvent(mesh) {
    }
    ~MeshCreatedEvent() override = default;
    DECLARE_MESH_EVENT(MeshCreated);
  };

  class MeshDestroyedEvent : public MeshEvent {
  public:
    explicit MeshDestroyedEvent(const Mesh* mesh):
      MeshEvent(mesh) {
    }
    ~MeshDestroyedEvent() override = default;
    DECLARE_MESH_EVENT(MeshDestroyed);
  };
}

#endif //MCC_MESH_EVENTS_H