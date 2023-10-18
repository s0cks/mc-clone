#ifndef MCC_FONT_MESH_H
#define MCC_FONT_MESH_H

#include "mcc/gfx.h"

namespace mcc::font {
  struct FontMesh {
  public:
    GLuint vao;
    GLuint vbo;

    FontMesh();
    ~FontMesh() = default;
  };
}

#endif //MCC_FONT_MESH_H