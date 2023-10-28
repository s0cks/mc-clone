#ifndef MCC_CUBE_H
#define MCC_CUBE_H

#include "mcc/mesh/mesh.h"

namespace mcc {
  class Cube {
  public:
    static mesh::Mesh* CreateMesh();
    static void InitVertices(std::vector<glm::vec3>& vertices);
    static void InitUv(std::vector<glm::vec3>& uv);
  };
}

#endif //MCC_CUBE_H