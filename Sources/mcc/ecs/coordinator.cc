#include "mcc/ecs/coordinator.h"
#include "mcc/thread_local.h"

namespace mcc {
  static ThreadLocal<Coordinator> local_;

  Coordinator* Coordinator::Initialize() {
    const auto value = new Coordinator();
    Set(value);
    return value;
  }

  Coordinator* Coordinator::Get() {
    return local_.Get();
  }

  void Coordinator::Set(Coordinator* value) {
    local_.Set(value);
  }
}