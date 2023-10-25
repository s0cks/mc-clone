#ifndef MCC_SQUARE_H
#define MCC_SQUARE_H

#include "mcc/shape/shape.h"
#include "mcc/scene.h"

namespace mcc {
  class Square : public Shape {
  public:
    static inline void
    GenerateVertices(VertexList& vertices, const float size = 0.5f) {
      vertices.push_back(Vertex {
        .pos = glm::vec3(size, size, 0.0f)
      });
      vertices.push_back(Vertex {
        .pos = glm::vec3(size, -size, 0.0f)
      });
      vertices.push_back(Vertex {
        .pos = glm::vec3(-size, -size, 0.0f)
      });
      vertices.push_back(Vertex {
        .pos = glm::vec3(-size, size, 0.5f)
      });
    }

    static inline void
    GenerateIndices(IndexList& indices) {
      indices.push_back(0);
      indices.push_back(1);
      indices.push_back(3);

      indices.push_back(1);
      indices.push_back(2);
      indices.push_back(3);
    }

  protected:
    static inline VertexList
    GenerateVertexList(const glm::vec2 pos, const float size) {
      return {
        { glm::vec3(pos[0] + size, pos[1] + size, 0.0f) }, // top right
        { glm::vec3(pos[0] + size, pos[1] - size, 0.0f) }, // bottom right
        { glm::vec3(pos[0] - size, pos[1] - size, 0.0f) }, // bottom left
        { glm::vec3(pos[0] - size, pos[1] + size, 0.0f) }, // top left
      };
    }

    static inline IndexList
    GenerateIndexList() {
      return {
        0, 1, 3, // first triangle
        1, 2, 3, // second triangle
      };
    }
  public:
    Square(const glm::vec2 pos,
           const float size = 0.5f):
      Shape(GetColorizedShader(), mesh::IndexedMesh::New(GenerateVertexList(pos, size), GenerateIndexList())) {
    }
    ~Square() override = default;
    DEFINE_NON_COPYABLE_TYPE(Square);
  };
}

#endif //MCC_SQUARE_H