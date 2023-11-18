#ifndef MCC_TERRAIN_CHUNK_H
#define MCC_TERRAIN_CHUNK_H

#include "mcc/gfx.h"
#include "mcc/pipeline.h"
#include "mcc/terrain/terrain_vertex.h"

namespace mcc::terrain {
  class TerrainChunk {
  protected:
    TerrainVertexBuffer vbo_;
    IndexBuffer ibo_;

    TerrainChunk(const VertexList& vertices, 
                 const IndexList& indices):
      vbo_(vertices),
      ibo_(indices) {
    }

    static void OnPreInit();
    static void OnPostInit();
  public:
    TerrainChunk() = delete;
    ~TerrainChunk() = default;

    glm::mat4 GetModelMatrix() const {
      return glm::mat4(1.0f);
    }

    void Render();

    TerrainVertexBuffer vbo() const {
      return vbo_;
    }

    IndexBuffer ibo() const {
      return ibo_;
    }
  public:
    static void Init();
    static TerrainChunk* New(const VertexList& vertices, const IndexList& indices);
  };

  class RenderTerrainChunkPipeline : public Pipeline {
  protected:
    TerrainChunk* chunk_;
    glm::mat4 model_;
  public:
    RenderTerrainChunkPipeline(TerrainChunk* chunk = nullptr,
                               const glm::mat4& model = glm::mat4(1.0f));
    ~RenderTerrainChunkPipeline() override = default;

    void Render() override;
  };
}

#endif //MCC_TERRAIN_CHUNK_H