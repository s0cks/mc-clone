#ifndef MCC_RENDERABLE_H
#define MCC_RENDERABLE_H

#include "mcc/mesh/mesh.h"
#include "mcc/shader/shader.h"
#include "mcc/texture/texture.h"

namespace mcc {
  struct Renderable {
    Shader shader;
    mesh::Mesh* mesh;
    texture::Texture texture;
  };
}

#endif //MCC_RENDERABLE_H