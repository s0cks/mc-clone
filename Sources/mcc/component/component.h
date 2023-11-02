#ifndef MCC_COMPONENT_H
#define MCC_COMPONENT_H

#include <cstdint>
#include <unordered_map>
#include <array>
#include <typeinfo>

#include "mcc/gfx.h"
#include "mcc/relaxed_atomic.h"
#include "mcc/ecs/entity.h"

#include "mcc/component/component_id.h"
#include "mcc/component/component_state.h"

namespace mcc {
#define DECLARE_COMPONENT(Name)                                                           \
  private:                                                                                \
    static void SetComponentId(const ComponentId);                                        \
    static void OnEntityDestroyed(EntityDestroyedEvent* e);                               \
public:                                                                                   \
    static ComponentId GetComponentId();                                                  \
    static void RegisterComponent();                                                      \
    static bool Visit(std::function<bool(const Entity& e, const ComponentState<Name>&)>); \
    static bool PutState(const Entity& e, const ComponentState<Name>& state);             \
    static bool RemoveState(const Entity& e);                                             \
    static std::optional<ComponentState<Name>> GetState(const Entity& e);

#define DEFINE_COMPONENT(Name)                                                                    \
  static RelaxedAtomic<ComponentId> component_id_(kInvalidComponentId);                           \
  static ComponentStateTable<Name> component_table_;                                              \
  void Name::SetComponentId(const ComponentId id) { component_id_ = id; }                         \
  ComponentId Name::GetComponentId() { return (ComponentId) component_id_; }                      \
  void Name::RegisterComponent() {                                                                \
    return SetComponentId(Component::Register<Name>());                                          \
  }                                                                                               \
  bool Name::PutState(const Entity& e, const ComponentState<Name>& state) {                       \
    return component_table_.Put(e, state);                                                        \
  }                                                                                               \
  bool Name::RemoveState(const Entity& e) {                                                       \
    return component_table_.Remove(e);                                                            \
  }                                                                                               \
  std::optional<ComponentState<Name>> Name::GetState(const Entity& e) {                           \
    return component_table_.Get(e);                                                               \
  }                                                                                               \
  void Name::OnEntityDestroyed(EntityDestroyedEvent* e) {                                         \
    LOG_IF(ERROR, !RemoveState(e->id)) << "failed to remove " << #Name << " from " << e->id;      \
  }                                                                                               \
  bool Name::Visit(std::function<bool(const Entity&, const ComponentState<Name>&)> vis) {         \
    return component_table_.Visit(vis);                                                           \
  }

  class Component {
    DEFINE_NON_INSTANTIABLE_TYPE(Component);
  private:
    template<typename T>
    static inline const char*
    TypeId() {
      return typeid(T).name();
    }

    static ComponentId Register(const char* name);
    static ComponentId GetComponentId(const char* name);
  public:
    template<typename T>
    static inline ComponentId Register() {
      return Register(TypeId<T>());
    }

    template<typename T>
    static inline ComponentId GetComponentId() {
      return GetComponentId(TypeId<T>());
    }
  };
}

#endif //MCC_COMPONENT_H