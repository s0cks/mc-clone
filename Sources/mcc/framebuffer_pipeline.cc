#include "mcc/framebuffer_pipeline.h"

namespace mcc {
  void RenderFrameBufferPipeline::Render() {
    dst_.Bind();
    if(HasClearMask()) {
      glClear(clear_mask_);
      CHECK_GL(FATAL);
    }
    
    TextureBindScope<0> tex(src_->tex());
    InvertedDepthTestScope depth_test;
    for(const auto& child : children_)
      child->Render();
    src_->mesh()->Draw(); //TODO: move to child
  }
}