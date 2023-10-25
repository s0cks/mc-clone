#ifndef MCC_PLANE_H
#define MCC_PLANE_H

#include "mcc/common.h"
#include "mcc/mesh/mesh.h"

namespace mcc {
  class Plane {
    DEFINE_NON_INSTANTIABLE_TYPE(Plane);
  private:
  public:
    static void GenerateVertices(VertexList& vertices, glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, int div);
  };
}

#endif //MCC_PLANE_H