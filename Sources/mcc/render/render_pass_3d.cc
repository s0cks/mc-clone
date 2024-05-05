#include "mcc/render/render_pass_3d.h"
#include "mcc/render/render_pass_terrain.h"

namespace mcc::render {
  RenderPass3d::RenderPass3d(Pipeline* pipeline):
    SequenceRenderPass(),
    pipeline_(pipeline) {
    Append(new TerrainRenderPass());
  }

  RenderPass3d::~RenderPass3d() {
    
  }
}