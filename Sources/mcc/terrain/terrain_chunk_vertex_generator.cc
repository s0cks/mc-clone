#include "mcc/terrain/terrain_chunk_vertex_generator.h"
#include "mcc/common.h"

namespace mcc::terrain {
  bool TerrainChunkVertexGenerator::Visit(TerrainChunk* chunk) {
    MCC_ASSERT(chunk);
    return true;
  }
}