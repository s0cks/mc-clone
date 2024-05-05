#ifndef MCC_TERRAIN_CHUNK_RENDER_PIPELINE_H
#define MCC_TERRAIN_CHUNK_RENDER_PIPELINE_H

#include "mcc/glm.h"
#include "mcc/render/render_pipeline.h"

namespace mcc::terrain {
  class TerrainChunkRenderPipeline : public render::RenderPipeline {
  public:
    TerrainChunkRenderPipeline(const glm::mat4& projection,
                               const glm::mat4& view,
                               const glm::mat4& model = glm::mat4(1.0f)):
      RenderPipeline(projection, view, model) {
    }
    virtual ~TerrainChunkRenderPipeline() = default;
  };
}

#endif //MCC_TERRAIN_CHUNK_RENDER_PIPELINE_H