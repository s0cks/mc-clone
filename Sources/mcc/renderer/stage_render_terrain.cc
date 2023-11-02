#include "mcc/renderer/stage.h"
#include "mcc/terrain/terrain.h"
#include "mcc/renderer/renderer.h"

namespace mcc::renderer { //TODO: merge w/ TerrainRenderer
  void RenderTerrainStage::Render(const Tick& tick, const glm::mat4& projection, const glm::mat4& view) {
    VLOG(3) << "rendering terrain....";
    glEnable(GL_DEPTH_TEST);
    CHECK_GL(FATAL);
    terrain::Terrain::Render(projection, view);
    glDisable(GL_DEPTH_TEST);
    CHECK_GL(FATAL);
  }
}