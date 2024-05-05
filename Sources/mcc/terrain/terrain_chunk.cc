#include "mcc/terrain/terrain_chunk.h"
#include "mcc/common.h"

namespace mcc::terrain {
  bool TerrainChunk::Accept(TerrainChunkVisitor* vis) {
    MCC_ASSERT(vis);
    return vis->Visit(this);
  }

  std::string TerrainChunk::ToString() const {
    std::stringstream ss;
    ss << "TerrainChunk(";
    ss << "id=" << GetId() << ", ";
    ss << "pos=" << glm::to_string(GetPos()) << ", ";
    ss << "size=" << glm::to_string(GetSize());
    ss << ")";
    return ss.str();
  }
}