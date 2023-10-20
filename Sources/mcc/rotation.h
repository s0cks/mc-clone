#ifndef MCC_ROTATION_H
#define MCC_ROTATION_H

#include "mcc/gfx.h"
#include "mcc/fixed_rate_loop.h"

namespace mcc {
  namespace scene {
    class NodeUpdater;
  }

  class Translation {
  protected:
    Translation() = default;
  public:
    virtual ~Translation() = default;
    virtual void Update(FixedRateLoop* updater) = 0;
    virtual glm::mat4 Apply(const glm::mat4 matrix) const = 0;
  };

  class Rotation : public Translation {
  public:
    float velocity_;
    glm::vec3 axis_;
    float angle_;
  public:
    Rotation(const float velocity, const glm::vec3 axis):
      Translation(),
      velocity_(velocity),
      axis_(axis),
      angle_(0) {
    }
    virtual ~Rotation() = default;
    void Update(FixedRateLoop* updater) override;

    glm::mat4 Apply(const glm::mat4 matrix) const override {
      return glm::rotate(matrix, glm::radians(-angle_), axis_);
    }
  };
}

#endif //MCC_ROTATION_H