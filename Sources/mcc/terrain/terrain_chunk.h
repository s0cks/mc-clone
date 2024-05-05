#ifndef MCC_TERRAIN_CHUNK_H
#define MCC_TERRAIN_CHUNK_H

namespace mcc::terrain {
  class TerrainChunk;
  class TerrainChunkVisitor {
    friend class TerrainChunk;
  protected:
    TerrainChunkVisitor() = default;
    virtual bool Visit(TerrainChunk* chunk) = 0;
  public:
    virtual ~TerrainChunkVisitor() = default;
  };

  class TerrainChunk {
  public:
    virtual ~TerrainChunk() = default;
    virtual bool Accept(TerrainChunkVisitor* vis);
  };
}

#endif //MCC_TERRAIN_CHUNK_H