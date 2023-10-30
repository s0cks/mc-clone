#include "mcc/renderer/renderable.h"
#include "mcc/relaxed_atomic.h"

namespace mcc::renderer {
  static RelaxedAtomic<ComponentId> id_;

  void Renderable::SetComponentId(const ComponentId id) {
    id_ = id;
  }

  ComponentId Renderable::GetComponentId() {
    return (ComponentId) id_;
  }
}