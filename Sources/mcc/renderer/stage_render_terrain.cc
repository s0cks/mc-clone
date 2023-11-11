#include "mcc/renderer/stage.h"
#include "mcc/terrain/terrain.h"
#include "mcc/renderer/renderer.h"

namespace mcc::renderer { //TODO: merge w/ TerrainRenderer
  void RenderTerrainStage::Render(const Tick& tick, const glm::mat4& projection, const glm::mat4& view) {
    VLOG(3) << "rendering terrain....";
    glDisable(GL_CULL_FACE);
    CHECK_GL(FATAL);
    terrain::Terrain::Render(projection, view);
    glEnable(GL_CULL_FACE);
    CHECK_GL(FATAL);
  }
}