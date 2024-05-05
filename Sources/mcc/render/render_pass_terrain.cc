#include "mcc/render/render_pass_terrain.h"

namespace mcc::render {
  bool TerrainRenderPass::ShouldSkip() const {
    NOT_IMPLEMENTED(ERROR); //TODO: implement
    return false;
  }

  void TerrainRenderPass::Render() {
    NOT_IMPLEMENTED(ERROR); //TODO: implement
    DLOG(INFO) << "rendering terrain.....";
  }
}