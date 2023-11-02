#include <unordered_map>
#include "mcc/ecs/component.h"

namespace mcc {
  static std::unordered_map<const char*, ComponentId> types_;
  static RelaxedAtomic<ComponentId> next_id_;

  void Components::Init() {
  }

  ComponentId Components::Register(const char* name) {
    const auto id = next_id_.fetch_add(1);
    types_.insert({ name, id });
    return id;
  }

  ComponentId Components::GetComponentId(const char* name) {
    return types_[name];
  }
}