#include "mcc/renderer/stage.h"
#include "mcc/terrain/terrain.h"
#include "mcc/renderer/renderer.h"

namespace mcc::renderer {
  void RenderTerrainStage::Render(const Tick& tick, const glm::mat4& projection, const glm::mat4& view) {
    DLOG(INFO) << "rendering terrain....";
    glEnable(GL_DEPTH_TEST);
    CHECK_GL(FATAL);
    terrain::Terrain::Render(projection, view);
    glDisable(GL_DEPTH_TEST);
    CHECK_GL(FATAL);
  }
}