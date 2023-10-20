#include "mcc/rotation.h"
#include "mcc/scene.h"
#include "mcc/mouse.h"

namespace mcc {
  void Rotation::Update(FixedRateLoop* updater) {
    if(Mouse::IsPressed(kMouseButton1))
      return;
    angle_ += velocity_;
  }
}