#ifndef MCC_SQUARE_H
#define MCC_SQUARE_H

#include "mcc/gfx.h"

namespace mcc::shape {
  class Square {
    DEFINE_NON_INSTANTIABLE_TYPE(Square);
  public:
    static inline void
    GenerateVertices(std::vector<glm::vec3>& vertices, glm::vec2 pos, glm::vec2 size) {
      vertices.push_back(glm::vec3(pos[0]          , pos[1]          , 0.0f));
      vertices.push_back(glm::vec3(pos[0]          , pos[1] + size[1], 0.0f));
      vertices.push_back(glm::vec3(pos[0] + size[0], pos[1] + size[1], 0.0f));
      vertices.push_back(glm::vec3(pos[0] + size[0], pos[1]          , 0.0f));
    }

    static inline void
    GenerateCenteredVertices(std::vector<glm::vec3>& vertices, glm::vec2 pos, glm::vec2 size) {
      const auto startX = pos[0] - (size[0] / 2);
      const auto startY = pos[1] - (size[1] / 2);
      return GenerateVertices(vertices, glm::vec2(startX, startY), size);
    }

    static inline void
    GenerateIndices(std::vector<GLubyte>& indices) {
      indices.push_back(0);
      indices.push_back(1);
      indices.push_back(3);

      indices.push_back(1);
      indices.push_back(2);
      indices.push_back(3);
    }
  };
}

#endif //MCC_SQUARE_H