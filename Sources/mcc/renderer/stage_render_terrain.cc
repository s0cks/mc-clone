#include "mcc/renderer/stage.h"
#include "mcc/terrain/terrain.h"
#include "mcc/renderer/renderer.h"

#include "mcc/skybox.h"

namespace mcc::renderer { //TODO: merge w/ TerrainRenderer
  void RenderTerrainStage::Render(const Tick& tick) {
    const auto skybox = skybox::Skybox::Get();
    if(skybox) {
      DLOG(INFO) << "rendering skybox....";
      skybox::RenderSkyboxPipeline skybox_pipe(skybox);
      skybox_pipe.Render();
    }

    const auto chunk = terrain::Terrain::GetChunk();
    if(chunk) {
      VLOG(3) << "rendering terrain....";
      terrain::RenderTerrainChunkPipeline pipeline(chunk);
      pipeline.Render();
      Renderer::IncrementVertexCounter(chunk->vbo().length());
    }    
  }
}