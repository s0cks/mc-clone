#include "mcc/gui/shape.h"

namespace mcc::shape {
  bool NewRect(const Dimension& size, d2::VertexList& vertices, u32::IndexList& indices) {
    if(size[0] <= 0 || size[1] <= 0) {
      DLOG(ERROR) << "invalid size: " << glm::to_string(size);
      return false;
    }
    const auto xSize = size[0];
    const auto ySize = size[1];
    const uint64_t total_indices = xSize * ySize * 6;
    const uint64_t total_vertices = (xSize + 1) * (ySize + 1);
    const glm::vec3 offset = glm::vec3(-1.0f, -1.0f, 0.0f);
    const float xStep = 2.0f / xSize;
    const float yStep = 2.0f / ySize;
    const float uStep = 1.0f / xSize;
    const float vStep = 1.0f / ySize;

    for(auto x = 0; x <= xSize; x++) {
      for(auto y = 0; y <= ySize; y++) {
        const auto i = y + x * (ySize + 1);
        vertices.push_back(d2::Vertex {
          .pos = glm::vec3(x * xStep, y * yStep, 0.0f) + offset,
          .uv = glm::vec2(x * uStep, (ySize - y) * vStep),
          .color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
        });
      }
    }
    for(auto v = 0; v < total_vertices - ySize - 2; v++) {
      if((v + 1) % (ySize + 1) == 0)
        v++;
      indices.push_back(v);
      indices.push_back(v + 1);
      indices.push_back(v + ySize + 1);
      indices.push_back(v + 1);
      indices.push_back(v + ySize + 2);
      indices.push_back(v + ySize + 1);
    }
    return true;
  }
}