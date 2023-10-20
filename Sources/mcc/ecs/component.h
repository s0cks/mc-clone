#ifndef MCC_COMPONENT_H
#define MCC_COMPONENT_H

#include <cstdint>
#include <unordered_map>
#include <array>
#include <typeinfo>

#include "mcc/gfx.h"
#include "mcc/ecs/entity.h"

namespace mcc {
  typedef uint64_t ComponentId;

  class ComponentList {
  protected:
    ComponentList() = default;
  public:
    virtual ~ComponentList() = default;
    virtual void OnDestroyed(const Entity e) = 0;
  public:
    template<typename T>
    static ComponentList* New();
  };
  
  template<typename T>
  class ComponentListTemplate : public ComponentList {
  protected:
    std::array<T, kMaxNumberOfEntities> components_;
    std::unordered_map<Entity, uint64_t> e2i_;
    std::unordered_map<uint64_t, Entity> i2e_;
    uint64_t size_;
  public:
    T& GetData(const Entity entity) {
      return components_[e2i_[entity]];
    }

    void InsertData(const Entity entity, const T component) {
      const auto new_idx = size_;
      e2i_[entity] = new_idx;
      i2e_[new_idx] = entity;
      components_[new_idx] = component;
      size_++;
    }

    void RemoveData(const Entity entity) {
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

    void OnDestroyed(const Entity e) override {
      if(e2i_.find(e) != e2i_.end())
        RemoveData(e);
    }
  };

  template<typename T>
  ComponentList* ComponentList::New() {
    return new ComponentListTemplate<T>();
  }

  class Components {
    DEFINE_NON_INSTANTIABLE_TYPE(Components);
  private:
    template<typename T>
    static inline const char*
    TypeId() {
      return typeid(T).name();
    }

    static void Register(const char* name, ComponentList* list);
    static ComponentList* GetComponentList(const char* name);
    static ComponentId GetComponentId(const char* name);
  public:
    template<typename T>
    static inline void Register() {
      return Register(TypeId<T>(), ComponentList::New<T>());
    }

    static void OnDestroyed(const Entity e);

    template<typename T>
    static inline T& GetComponent(const Entity e) {
      return GetComponentListForType<T>()->GetData(e);
    }

    template<typename T>
    static inline void RemoveComponent(const Entity e) {
      return GetComponentListForType<T>()->RemoveData(e);
    }

    template<typename T>
    static inline void AddComponent(const Entity e, T component) {
      return GetComponentListForType<T>()->InsertData(e, component);
    }

    template<typename T>
    static inline ComponentId GetComponentIdForType() {
      return GetComponentId(TypeId<T>());
    }

    template<typename T>
    static inline ComponentListTemplate<T>* 
    GetComponentListForType() {
      return reinterpret_cast<ComponentListTemplate<T>*>(GetComponentList(TypeId<T>()));
    }
  };

  class Coordinator;
  class ComponentManager {
    friend class Coordinator;
  protected:
    std::unordered_map<const char*, ComponentId> types_;
    std::unordered_map<const char*, ComponentList*> components_;
    ComponentId next_id_;

    ComponentManager() = default;
  public:
    virtual ~ComponentManager() = default;
  };
}

#endif //MCC_COMPONENT_H