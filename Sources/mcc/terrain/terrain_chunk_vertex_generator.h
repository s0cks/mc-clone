#ifndef MCC_TERRAIN_CHUNK_VERTEX_GENERATOR_H
#define MCC_TERRAIN_CHUNK_VERTEX_GENERATOR_H

#include "mcc/common.h"
#include "mcc/terrain/terrain_chunk.h"
#include "mcc/terrain/terrain_vertex.h"

#include "mcc/shape.h"

namespace mcc::terrain {
  class TerrainChunkVertexGenerator : public TerrainChunkVisitor {
  protected:
    VertexList& vertices_;

    bool Visit(TerrainChunk* chunk) override {
      
    }
  public:
    explicit TerrainChunkVertexGenerator(VertexList& vertices):
      TerrainChunkVisitor(),
      vertices_(vertices) {
    }
    ~TerrainChunkVertexGenerator() override = default;
  };
}

#endif //MCC_TERRAIN_CHUNK_VERTEX_GENERATOR_H