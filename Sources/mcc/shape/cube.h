#ifndef MCC_CUBE_H
#define MCC_CUBE_H

#include "mcc/mesh/mesh.h"

namespace mcc {
  class Cube {
  private:
    static void InitializeVertices(VertexList& list);
  public:
    static mesh::Mesh* CreateMesh();
  };
}

#endif //MCC_CUBE_H