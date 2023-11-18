#include "mcc/framebuffer_pipeline.h"

namespace mcc {
  void FrameBufferPipeline::Render() {
    dst_.Bind();
    glPolygonMode(polygon_mode_.face, polygon_mode_.mode);
    CHECK_GL(FATAL);
    glClear(clear_mask_);
    CHECK_GL(FATAL);
    glClearColor(clear_color_[0],clear_color_[1], clear_color_[2], clear_color_[3]);
    CHECK_GL(FATAL);
    glEnable(caps_);
    CHECK_GL(FATAL);
    if(IsDepthTestEnabled()) {
      glDepthFunc(depth_func_);
      CHECK_GL(FATAL);
    }

    if(IsCullFaceEnabled()) {
      glFrontFace(front_face_);
      CHECK_GL(FATAL);
      glCullFace(cull_face_);
      CHECK_GL(FATAL);
    }

    RenderChildren();
  }

  void RenderFrameBufferPipeline::Render() {
    dst_.Bind();
    if(HasClearMask()) {
      glClear(clear_mask_);
      CHECK_GL(FATAL);
    }
    
    InvertedDepthTestScope depth_test;
    for(const auto& child : children_)
      child->Render();
    FrameBuffer::GetMesh()->Draw();
  }
}