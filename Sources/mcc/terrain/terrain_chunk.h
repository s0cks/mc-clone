#ifndef MCC_TERRAIN_CHUNK_H
#define MCC_TERRAIN_CHUNK_H

#include "mcc/glm.h"

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

  typedef uint64_t ChunkId;

  class TerrainChunk {
  protected:
    ChunkId id_;
    glm::vec3 pos_;
    glm::vec2 size_;
  public:
    TerrainChunk(const ChunkId id,
                 const glm::vec3& pos,
                 const glm::vec2& size):
      id_(id),
      pos_(pos),
      size_(size) {
    }
    virtual ~TerrainChunk() = default;
    virtual std::string ToString() const;
    virtual bool Accept(TerrainChunkVisitor* vis);

    ChunkId GetId() const {
      return id_;
    }

    const glm::vec3& GetPos() const {
      return pos_;
    }

    const glm::vec2& GetSize() const {
      return size_;
    }
  };
}

#endif //MCC_TERRAIN_CHUNK_H