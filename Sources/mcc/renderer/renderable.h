#ifndef MCC_RENDERABLE_H
#define MCC_RENDERABLE_H

#include "mcc/mesh/mesh.h"
#include "mcc/shader/shader.h"

namespace mcc {
  struct Renderable {
    Shader shader;
    Mesh* mesh;
    glm::vec3 color;
  };
}

#endif //MCC_RENDERABLE_H