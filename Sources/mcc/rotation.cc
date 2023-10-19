#include "mcc/rotation.h"
#include "mcc/scene.h"

namespace mcc {
  void Rotation::Update(FixedRateLoop* updater) {
    angle_ += velocity_;
  }
}