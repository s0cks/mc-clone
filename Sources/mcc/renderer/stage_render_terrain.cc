#include "mcc/renderer/stage.h"
#include "mcc/terrain/terrain.h"
#include "mcc/renderer/renderer.h"

namespace mcc::renderer { //TODO: merge w/ TerrainRenderer
  void RenderTerrainStage::Render(const Tick& tick) {
    const auto camera = camera::PerspectiveCameraBehavior::GetCameraComponent();
    const auto view = glm::mat4(glm::mat3((*camera)->GetViewMatrix()));
    Renderer::GetCameraUniformBuffer()->UpdateView(view);
    const auto skybox = skybox::Skybox::Get();
    skybox->Render();
    Renderer::GetCameraUniformBuffer()->UpdateView((*camera)->GetViewMatrix());

    VLOG(3) << "rendering terrain....";
    glDisable(GL_CULL_FACE);
    CHECK_GL(FATAL);
    terrain::Terrain::Render();
    Renderer::IncrementVertexCounter(terrain::Terrain::GetChunk()->vbo().length());
    glEnable(GL_CULL_FACE);
    CHECK_GL(FATAL);
  }
}