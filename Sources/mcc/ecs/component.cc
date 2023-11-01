#include "mcc/ecs/component.h"

namespace mcc {
  static std::unordered_map<const char*, ComponentId> types_;
  static std::unordered_map<const char*, ComponentList*> components_;
  static ComponentId next_id_;

  void Components::Init() {
    Entity::OnDestroyed()
      .subscribe([](EntityDestroyedEvent* e) {
        for(const auto pair : components_) {
          const auto component = pair.second;
          component->OnDestroyed(e->id);
        }
      });
  }

  ComponentId Components::Register(const char* name, ComponentList* list) {
    types_.insert({ name, next_id_ });
    components_.insert({ name, list });
    const auto id = next_id_;
    next_id_++;
    return id;
  }

  ComponentList* Components::GetComponentList(const char* name) {
    return components_[name];
  }

  ComponentId Components::GetComponentId(const char* name) {
    return types_[name];
  }
}