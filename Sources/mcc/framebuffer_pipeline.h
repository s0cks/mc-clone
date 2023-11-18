#ifndef MCC_FRAMEBUFFER_PIPELINE_H
#define MCC_FRAMEBUFFER_PIPELINE_H

#include "mcc/gfx.h"
#include "mcc/pipeline.h"
#include "mcc/framebuffer.h"
#include "mcc/shader/shader_pipeline.h"

namespace mcc {
  enum ClearMask : GLbitfield {
    kNoClearMask = 0,
    kColorBufferClearMask = GL_COLOR_BUFFER_BIT,
    kDepthBufferClearMask = GL_DEPTH_BUFFER_BIT,
    kColorAndDepthClearMask = kColorBufferClearMask|kDepthBufferClearMask,
    kDefaultClearMask = kColorAndDepthClearMask,
  };

  class FrameBufferPipeline : public Pipeline {
  private:
    FrameBufferObject src_;
    FrameBufferObject dst_;
    ClearMask clear_mask_;
    glm::vec4 clear_color_;
  public:
    FrameBufferPipeline(FrameBufferObject src,
                        FrameBufferObject dst,
                        const ClearMask clear_mask = kDefaultClearMask,
                        const glm::vec4& clear_color = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f)):
      Pipeline(),
      src_(src),
      dst_(dst),
      clear_mask_(clear_mask),
      clear_color_(clear_color) {
    }
    ~FrameBufferPipeline() override = default;

    FrameBufferObject src() const {
      return src_;
    }

    FrameBufferObject dst() const {
      return dst_;
    }

    void Render() override {
      dst_.Bind();
      glClear(clear_mask_);
      glClearColor(clear_color_[0],clear_color_[1], clear_color_[2], clear_color_[3]);
      for(const auto& child : children_)
        child->Render();
      src_.Bind();
    }
  };

  class RenderFrameBufferPipeline : public Pipeline {
  private:
    FrameBuffer* src_;
    FrameBufferObject dst_;
    ShaderRef shader_;
    ClearMask clear_mask_;

    inline bool HasClearMask() {
      return clear_mask_ != kNoClearMask;
    }
  public:
    RenderFrameBufferPipeline(FrameBuffer* src,
                              FrameBufferObject dst,
                              ApplyShaderPipeline* shader = nullptr,
                              const ClearMask clear_mask = kNoClearMask):
      Pipeline(),
      src_(src),
      dst_(dst),
      shader_(shader ? shader->shader() : ShaderRef()),
      clear_mask_(clear_mask) {
      if(shader)
        AddChild(shader);
    }
    RenderFrameBufferPipeline(FrameBuffer* src,
                              const BufferObjectId dst,
                              ApplyShaderPipeline* shader,
                              const ClearMask clear_mask = kNoClearMask):
      Pipeline(),
      src_(src),
      dst_(dst),
      shader_(shader->shader()),
      clear_mask_(clear_mask) {
      if(shader)
        AddChild(shader);
    }
    RenderFrameBufferPipeline(FrameBuffer* src,
                              FrameBufferObject dst,
                              ShaderRef shader,
                              const ClearMask clear_mask = kNoClearMask):
      Pipeline(),
      src_(src),
      dst_(dst),
      shader_(shader),
      clear_mask_(clear_mask) {
      AddChild(new ApplyShaderPipeline(shader, [](const ShaderRef& s) {
        s->SetInt("tex", 0);
        s->SetInt("bloomTex", 1);
        s->SetBool("bloom", true);
        s->SetBool("hdr", true);
        s->SetFloat("gamma", 2.2f);
        s->SetFloat("exposure", 1.0f);
      }));
    }
    RenderFrameBufferPipeline(FrameBuffer* src,
                              FrameBuffer* dst,
                              ShaderRef shader,
                              const ClearMask clear_mask = kNoClearMask):
      RenderFrameBufferPipeline(src, dst->fbo(), shader, clear_mask) {
    }
    RenderFrameBufferPipeline(FrameBuffer* src,
                              FrameBuffer* dst,
                              ApplyShaderPipeline* shader,
                              const ClearMask clear_mask = kNoClearMask):
      RenderFrameBufferPipeline(src, dst->fbo(), shader, clear_mask) {
    }
    
    ~RenderFrameBufferPipeline() override = default;

    void Render() override;
  };
}

#endif //MCC_FRAMEBUFFER_PIPELINE_H