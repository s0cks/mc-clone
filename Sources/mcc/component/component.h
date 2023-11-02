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

  class ComponentList {
  protected:
    ComponentList() = default;
  public:
    virtual ~ComponentList() = default;
    virtual void OnDestroyed(const Entity& e) = 0;
  public:
    template<typename T>
    static ComponentList* New();
  };
  
  template<typename T>
  class ComponentListTemplate : public ComponentList {
  protected:
    std::array<ComponentState<T>, kMaxNumberOfEntities> components_;
    std::unordered_map<Entity, uint64_t, Entity::HashFunction> e2i_;
    std::unordered_map<uint64_t, Entity> i2e_;
    uint64_t size_;
  public:
    void ForEachEntityAndComponent(std::function<void(const Entity&, const ComponentState<T>&)> callback) {
      for(auto idx = 0; idx < size_; idx++) {
        callback(i2e_[idx], components_[idx]);
      }
    }

    ComponentState<T> GetData(const Entity& entity) {
      return components_[e2i_[entity]];
    }

    void InsertData(const Entity& entity, T* component) {
      const auto new_idx = size_;
      e2i_[entity] = new_idx;
      i2e_[new_idx] = entity;
      components_[new_idx] = component;
      size_++;
    }

    void InsertData(const Entity& e, const T& component) {
      return InsertData(e, new T(component));
    }

    void RemoveData(const Entity& entity) {
      auto idx = e2i_[entity];
      auto last_idx = size_ - 1;
      components_[idx] = components_[last_idx];

      const auto last_entity = i2e_[last_idx];
      e2i_[last_entity] = idx;
      i2e_[idx] = last_entity;

      e2i_.erase(entity);
      i2e_.erase(last_idx);
      size_--;
    }

    void OnDestroyed(const Entity& e) override {
      if(e2i_.find(e) != e2i_.end())
        RemoveData(e);
    }
  };

  template<typename T>
  ComponentList* ComponentList::New() {
    return new ComponentListTemplate<T>();
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