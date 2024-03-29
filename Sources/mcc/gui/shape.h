#ifndef MCC_SHAPE_H
#define MCC_SHAPE_H

#include "mcc/vertex.h"
#include "mcc/index_buffer.h"

namespace mcc::shape {
  typedef glm::u32vec2 Dimension;

  bool NewRect(const Dimension& size, d2::VertexList& vertices, u32::IndexList& indices);

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
 }

#endif //MCC_SHAPE_H