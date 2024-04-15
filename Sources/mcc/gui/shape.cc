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
    const glm::vec2 offset = glm::vec2(-1.0f, -1.0f);
    const float xStep = 2.0f / xSize;
    const float yStep = 2.0f / ySize;
    const float uStep = 1.0f / xSize;
    const float vStep = 1.0f / ySize;

    for(auto x = 0; x <= xSize; x++) {
      for(auto y = 0; y <= ySize; y++) {
        const auto pos = glm::vec2(x * xStep, y * yStep) + offset;
        vertices.push_back(d2::Vertex {
          .pos = pos,
          .uv = glm::vec2(x * uStep, (ySize - y) * vStep),
          .color = glm::u8vec4(255, 0, 0, 255),
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

  bool NewRect(const glm::vec2 pos, const glm::vec2 size, d2::VertexList& vertices, u32::IndexList& indices) {
    const auto& x = pos[0];
    const auto& y = pos[1];
    const auto& width = size[0];
    const auto& height = size[1];
    const auto start = vertices.size();
    vertices.push_back({
      .pos = glm::vec2(x, y + height),
      .color = glm::u8vec4(255, 0, 0, 255),
    });
    vertices.push_back({
      .pos = glm::vec2(x, y),
      .color = glm::u8vec4(255, 0, 0, 255),
    });
    vertices.push_back({
      .pos = glm::vec2(x + width, y),
      .color = glm::u8vec4(255, 0, 0, 255),
    });
    vertices.push_back({
      .pos = glm::vec2(x + width, y + height),
      .color = glm::u8vec4(255, 0, 0, 255),
    });
    indices.push_back(start);
    indices.push_back(start + 1);
    indices.push_back(start + 2);
    indices.push_back(start);
    indices.push_back(start + 2);
    indices.push_back(start + 3);
    return true;
  }

  bool NewGrid(const int slices, d2::VertexList& vertices, u32::IndexList& indices) {
    for(auto j = 0; j <= slices; j++) {
      for(auto i = 0; i <= slices; i++) {
        const float x = (i / slices) * 100;
        const float y = (j / slices) * 100;
        vertices.push_back(d2::Vertex {
          .pos = glm::vec2(x, y),
          .color = glm::u8vec4(0, 0, 0, 255),
        });
      }
    }

    for(auto j = 0; j < slices; j++) {
      for(auto i = 0; i < slices; i++) {
        const int r1 = j * (slices + 1);
        const int r2 = (j + 1) * (slices + 1);
        indices.push_back(r1 + i);
        indices.push_back(r1 + i + 1);
        indices.push_back(r1 + i + 1);
        indices.push_back(r2 + i + 1);

        indices.push_back(r2 + i + 1);
        indices.push_back(r2 + i);
        indices.push_back(r2 + i);
        indices.push_back(r1 + i);
      }
    }
    return true;
  }
}