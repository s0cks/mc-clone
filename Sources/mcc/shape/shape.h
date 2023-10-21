#ifndef MCC_SHAPE_H
#define MCC_SHAPE_H

#include "mcc/mesh/mesh.h"
#include "mcc/shader/shader.h"
#include "mcc/scene.h"
#include "mcc/mesh/model.h"

namespace mcc {
  class Shape : public Model {
  protected:
    Shape() = default;
    Shape(const Shader& shader,
          mesh::Mesh* mesh):
          Model(shader, mesh) {
    }
  public:
    ~Shape() override = default;
  };
}

#endif //MCC_SHAPE_H