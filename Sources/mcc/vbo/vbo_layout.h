#ifndef MCC_VBO_LAYOUT_H
#define MCC_VBO_LAYOUT_H

#include "mcc/uv.h"
#include "mcc/color.h"

namespace mcc::vbo {
  struct Colored2D {
    glm::vec2 pos;
    Color color;
  };

  struct Textured2D {
    glm::vec2 pos;
    TexCoord uv;
  };

  struct Colored3D {
    glm::vec3 pos;
    Color color;
  };

  struct Textured3D {
    glm::vec3 pos;
    TexCoord uv;
  };
}

#endif //MCC_VBO_LAYOUT_H