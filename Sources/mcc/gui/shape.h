#ifndef MCC_SHAPE_H
#define MCC_SHAPE_H

#include "mcc/vertex/vertex.h"
#include "mcc/index_buffer.h"

namespace mcc::shape {
  typedef glm::u32vec2 Dimension;

  bool NewRect(const glm::vec2 pos, const Dimension size, d2::VertexList& vertices, u32::IndexList& indices);
  bool NewRect(const Dimension& size, d2::VertexList& vertices, u32::IndexList& indices);

  bool NewRect(const glm::vec2 pos, const glm::vec2 size, d2::VertexList& vertices, u32::IndexList& indices);

  static inline d2::Mesh*
  NewRectMesh(const Dimension& size) {
    d2::VertexList vertices;
    u32::IndexList indices;
    if(!NewRect(size, vertices, indices)) {
      DLOG(ERROR) << "failed to generate new rect mesh.";
      return nullptr;
    }
    return nullptr;
  }

  bool NewGrid(const int slices, d2::VertexList& vertices, u32::IndexList& indices);
 }

#endif //MCC_SHAPE_H