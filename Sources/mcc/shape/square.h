#ifndef MCC_SQUARE_H
#define MCC_SQUARE_H

#include "mcc/shape/shape.h"
#include "mcc/scene.h"

namespace mcc {
  class Square : public Shape {
  protected:
    static inline VertexList
    GenerateVertices(const glm::vec2 pos, const float size) {
      return {
        { glm::vec3(pos[0] + size, pos[1] + size, 0.0f) }, // top right
        { glm::vec3(pos[0] + size, pos[1] - size, 0.0f) }, // bottom right
        { glm::vec3(pos[0] - size, pos[1] - size, 0.0f) }, // bottom left
        { glm::vec3(pos[0] - size, pos[1] + size, 0.0f) }, // top left
      };
    }

    static inline IndexList
    GenerateIndices() {
      return {
        0, 1, 3, // first triangle
        1, 2, 3, // second triangle
      };
    }
  public:
    Square(const glm::vec2 pos,
           const float size = 0.5f):
      Shape(GetColorizedShader(), new Mesh(GenerateVertices(pos, size), GenerateIndices())) {
    }
    ~Square() override = default;
    DEFINE_NON_COPYABLE_TYPE(Square);
  };
}

#endif //MCC_SQUARE_H