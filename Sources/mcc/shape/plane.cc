#include "mcc/shape/plane.h"

namespace mcc {
  void Plane::GenerateVertices(VertexList& vertices, glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, int div) {
    glm::vec3 v03 = (v3 - v0) / static_cast<float>(div);
    glm::vec3 v12 = (v2 - v1) / static_cast<float>(div);
    for(auto row = 0.0f; row < div + 1.0f; row++) {
      glm::vec3 start = v0 + v03 * row;
      glm::vec3 end = v1 + v12 * row;
      glm::vec3 vdiv = (end - start) / static_cast<float>(div);
      for(auto col = 0.0f; col <  col + 1.0f; col++) {
        glm::vec3 pos = start + vdiv * col;
        glm::vec2 tex = glm::vec2(col / div, row / div);
        glm::vec3 tangent = glm::normalize(vdiv);
        glm::vec3 next_start = start + v03;
        glm::vec3 next_end = end + v12;
        glm::vec3 next_vdiv = (next_end - next_start) / static_cast<float>(div);
        glm::vec3 next_row = (next_start + next_vdiv) / static_cast<float>(div);
        glm::vec3 bitangent = glm::normalize(next_row - pos);
        glm::vec3 normal = glm::cross(tangent, bitangent);
        vertices.push_back(Vertex{
          .pos = pos,
          .tex = tex,
          .normal = normal,
          .bitangent = bitangent,
          .tangent = tangent,
        });
      }
    }
  }
}