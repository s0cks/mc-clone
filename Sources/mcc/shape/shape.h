#ifndef MCC_SHAPE_H
#define MCC_SHAPE_H

#include "mcc/mesh/mesh.h"
#include "mcc/shader/shader.h"

namespace mcc {
  class Shape {
  protected:
    Shape() = default;
  public:
    virtual ~Shape() = default;
    virtual void Render() = 0;
  };
}

#endif //MCC_SHAPE_H