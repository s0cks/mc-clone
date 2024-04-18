#include "mcc/transform.h"

#include "mcc/thread_local.h"
#include "mcc/engine/engine.h"

namespace mcc {
  static ThreadLocal<TransformComponent> component_;

  static inline void
  Set(TransformComponent* component) {
    MCC_ASSERT(component);
    component_.Set(component);
  }

  void TransformComponent::Init() {
    DLOG(INFO) << "initializing....";
    Set(new TransformComponent());
  }

  TransformComponent* TransformComponent::Get() {
    return component_.Get();
  }
}