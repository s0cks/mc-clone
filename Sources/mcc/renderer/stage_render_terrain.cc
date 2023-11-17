#include "mcc/renderer/stage.h"
#include "mcc/terrain/terrain.h"
#include "mcc/renderer/renderer.h"

#include "mcc/skybox.h"

namespace mcc::renderer { //TODO: merge w/ TerrainRenderer
  void RenderTerrainStage::Render(const Tick& tick) {
    const auto camera = camera::PerspectiveCameraBehavior::GetCameraComponent();
    const auto skybox = skybox::Skybox::Get();
    if(skybox) {
      const auto view = glm::mat4(glm::mat3((*camera)->GetViewMatrix()));
      Renderer::GetCameraUniformBuffer()->UpdateView(view);
      skybox::RenderSkyboxPipeline skybox_pipe(skybox);
      skybox_pipe.Render();
      Renderer::GetCameraUniformBuffer()->UpdateView((*camera)->GetViewMatrix());
    }

    VLOG(3) << "rendering terrain....";
    InvertedCullFaceScope cull_face;
    terrain::Terrain::Render();
    Renderer::IncrementVertexCounter(terrain::Terrain::GetChunk()->vbo().length());
  }
}