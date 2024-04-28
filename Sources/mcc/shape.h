#ifndef MCC_SHAPE_H
#define MCC_SHAPE_H

#include <vector>
#include "mcc/glm.h"
#include "mcc/color.h"

namespace mcc {
  namespace shape {
    template<typename Vertex, typename Index>
    void NewQuad(std::vector<Vertex>& vertices,
                 std::vector<Index>& indices,
                 const glm::vec2& topLeft,
                 const glm::vec2& bottomLeft,
                 const glm::vec2& bottomRight,
                 const glm::vec2& topRight,
                 const Color& color = kBlack) {
      const auto vertex_start = vertices.size();
      vertices.push_back(Vertex {
        .pos = glm::vec3(topLeft, 0.0f),
        .color = color,
      });
      vertices.push_back(Vertex {
        .pos = glm::vec3(bottomLeft, 0.0f),
        .color = color,
      });
      vertices.push_back(Vertex {
        .pos = glm::vec3(bottomRight, 0.0f),
        .color = color,
      });
      vertices.push_back(Vertex {
        .pos = glm::vec3(topLeft, 0.0f),
        .color = color,
      });
      vertices.push_back(Vertex {
        .pos = glm::vec3(bottomRight, 0.0f),
        .color = color,
      });
      vertices.push_back(Vertex {
        .pos = glm::vec3(topRight, 0.0f),
        .color = color,
      });
      const auto v1 = vertex_start + 0;
      const auto v2 = vertex_start + 1;
      const auto v3 = vertex_start + 2;
      const auto v4 = vertex_start + 3;
      // // triangle 1
      // indices.push_back(v1);
      // indices.push_back(v2);
      // indices.push_back(v3);
      // // triangle 2
      // indices.push_back(v1);
      // indices.push_back(v3);
      // indices.push_back(v4);
    }

#define X pos.x
#define Y pos.y
#define WIDTH size[0]
#define HEIGHT size[1]
#define HALF_WIDTH (WIDTH / 2)
#define HALF_HEIGHT (HEIGHT / 2)

    template<typename Vertex, typename Index>
    void NewRect(std::vector<Vertex>& vertices,
                 std::vector<Index>& indices,
                 const glm::vec2& pos,
                 const glm::vec2& size,
                 const Color& color = kBlack) {
      const auto topLeft = glm::vec2(X, Y + HEIGHT);
      const auto bottomLeft = glm::vec2(X, Y);
      const auto bottomRight = glm::vec2(X + WIDTH, Y);
      const auto topRight = glm::vec2(X + WIDTH, Y + HEIGHT);
      return NewQuad<Vertex, Index>(vertices, indices, topLeft, bottomLeft, bottomRight, topRight, color);
    }

    template<typename Vertex, typename Index>
    void NewCenteredRect(std::vector<Vertex>& vertices,
                         std::vector<Index>& indices,
                         const glm::vec2& pos,
                         const glm::vec2& size,
                         const Color& color = kBlack) {
      const auto topLeft = glm::vec2(X - HALF_WIDTH, Y + HALF_HEIGHT);
      const auto bottomLeft = glm::vec2(X - HALF_WIDTH, Y - HALF_HEIGHT);
      const auto bottomRight = glm::vec2(X + HALF_WIDTH, Y - HALF_HEIGHT);
      const auto topRight = glm::vec2(X + HALF_WIDTH, Y + HALF_HEIGHT);
      return NewQuad<Vertex, Index>(vertices, indices, topLeft, bottomLeft, bottomRight, topRight, color);
    }
  }
}

#endif //MCC_SHAPE_H