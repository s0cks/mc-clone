#ifndef MCC_SQUARE_H
#define MCC_SQUARE_H

#include "mcc/shape/shape.h"

namespace mcc {
  class Square : public Shape {
  protected:
    Shader shader_;
    Mesh* mesh_;

    static inline VertexList
    GenerateVertices(const glm::vec2 pos, const float size) {
      return {
        { glm::vec3(size, size, 0.0f) }, // top right
        { glm::vec3(size, -size, 0.0f) }, // bottom right
        { glm::vec3(-size, -size, 0.0f) }, // bottom left
        { glm::vec3(-size, size, 0.0f) }, // top left
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
      Shape(),
      shader_(GetColorizedShader()),
      mesh_(new Mesh(GenerateVertices(pos, size), GenerateIndices()))  {
    }
    ~Square() override {
      delete mesh_;
    }

    Mesh* GetMesh() const {
      return mesh_;
    }

    Shader GetShader() const {
      return shader_;
    }

    void Render() override;
  };
}

#endif //MCC_SQUARE_H