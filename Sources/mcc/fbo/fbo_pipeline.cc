#include "mcc/fbo/fbo_pipeline.h"

namespace mcc::fbo {
  RenderFboPipeline* RenderFboPipeline::New(const Fbo* fbo) {
    return new RenderFboPipeline(fbo);
  }
}