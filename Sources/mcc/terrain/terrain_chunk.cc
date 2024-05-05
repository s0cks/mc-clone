#include "mcc/terrain/terrain_chunk.h"
#include "mcc/common.h"

namespace mcc::terrain {
  bool TerrainChunk::Accept(TerrainChunkVisitor* vis) {
    MCC_ASSERT(vis);
    return vis->Visit(this);
  }
}