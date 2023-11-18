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

  struct PolygonMode {
    enum Face {
      kFront = GL_FRONT,
      kBack = GL_BACK,
      kFrontAndBack = GL_FRONT_AND_BACK,
      kDefaultFace = kFrontAndBack,
    };

    enum Mode {
      kFill = GL_FILL,
      kLine = GL_LINE,
      kWireframe = kLine,
      kDefaultMode = kFill,
    };

    Face face;
    Mode mode;
  };
  static constexpr const auto kDefaultPolygonMode = PolygonMode {
    .face = PolygonMode::kDefaultFace,
    .mode = PolygonMode::kDefaultMode,
  };
  static constexpr const auto kDefaultClearColor = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f);

  class FrameBufferPipeline : public Pipeline {
  public:
    static constexpr const auto kDefaultDepthFunction = GL_LEQUAL;
    static constexpr const auto kDefaultFrontFace = GL_CW;
    static constexpr const auto kDefaultCullFace = GL_BACK;
  private:
    FrameBufferObject dst_;
    PolygonMode polygon_mode_;
    ClearMask clear_mask_;
    glm::vec4 clear_color_;
    GLenum caps_;
    GLenum depth_func_;
    GLenum cull_face_;
    GLenum front_face_;
  public:
    FrameBufferPipeline(FrameBufferObject dst):
      Pipeline(),
      dst_(dst),
      caps_(GL_DEPTH_TEST|GL_CULL_FACE),
      depth_func_(kDefaultDepthFunction),
      cull_face_(kDefaultCullFace),
      front_face_(kDefaultFrontFace),
      polygon_mode_(kDefaultPolygonMode),
      clear_mask_(kDefaultClearMask),
      clear_color_(kDefaultClearColor) {
    }
    ~FrameBufferPipeline() override = default;

    void SetCapabilities(const GLenum caps) {
      caps_ = caps;
    }

    GLenum GetCapabilities() const {
      return caps_;
    }

    GLenum GetDepthFunction() const {
      return depth_func_;
    }

    void SetDepthFunction(const GLenum func) {
      depth_func_ = func;
    }

    GLenum GetCullFace() const {
      return cull_face_;
    }

    void SetCullFace(const GLenum value) {
      cull_face_ = value;
    }

    GLenum GetFrontFace() const {
      return front_face_;
    }

    void SetFrontFace(const GLenum value) {
      front_face_ = value;
    }
    
    bool IsDepthTestEnabled() const {
      return (caps_ & GL_DEPTH_TEST) == GL_DEPTH_TEST;
    }

    bool IsCullFaceEnabled() const {
      return (caps_ & GL_CULL_FACE) == GL_CULL_FACE;
    }

    FrameBufferObject dst() const {
      return dst_;
    }

    ClearMask GetClearMask() const {
      return clear_mask_;
    }

    PolygonMode GetPolygonMode() const {
      return polygon_mode_;
    }

    glm::vec4 GetClearColor() const {
      return clear_color_;
    }

    void Render() override;
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
                              ApplyShaderPipeline* shader = nullptr,
                              const ClearMask clear_mask = kNoClearMask):
      RenderFrameBufferPipeline(src, dst->fbo(), shader, clear_mask) {
    }
    RenderFrameBufferPipeline(FrameBuffer* src,
                              ApplyShaderPipeline* shader = nullptr,
                              const ClearMask clear_mask = kNoClearMask):
      RenderFrameBufferPipeline(src, FrameBufferObject(kDefaultFrameBufferObjectId), shader, clear_mask) {
    }
    RenderFrameBufferPipeline(FrameBuffer* src,
                              const ClearMask clear_mask = kNoClearMask):
      RenderFrameBufferPipeline(src, nullptr, clear_mask) {
    }
    
    ~RenderFrameBufferPipeline() override = default;

    void Render() override;
  };
}

#endif //MCC_FRAMEBUFFER_PIPELINE_H