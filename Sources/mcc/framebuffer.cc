#include "mcc/framebuffer.h"
#include "mcc/engine/engine.h"
#include "mcc/shader/shader.h"

namespace mcc {
  static const d2::VertexList kFrameBufferVertices = {
    { .pos = glm::vec2(1.0f, -1.0f), .uv = glm::vec2(1.0f, 0.0f) },
    { .pos = glm::vec2(-1.0f, -1.0f), .uv = glm::vec2(0.0f, 0.0f) },
    { .pos = glm::vec2(-1.0f, 1.0f), .uv = glm::vec2(0.0f, 1.0f) },

    { .pos = glm::vec2(1.0f, 1.0f), .uv = glm::vec2(1.0f, 1.0f) },
    { .pos = glm::vec2(1.0f, -1.0f), .uv = glm::vec2(1.0f, 0.0f) },
    { .pos = glm::vec2(-1.0f, 1.0f), .uv = glm::vec2(0.0f, 1.0f) },
  };
  static d2::Mesh* kFrameBufferMesh;
  
  static inline TextureRef
  CreateColorBuffer(const Dimension& size, ColorAttachment attachment) {
    DLOG(INFO) << "creating color buffer #" << attachment.slot;
    const auto texture = new texture::Texture(texture::k2D, true, true, false, attachment.alignment, attachment.filter, attachment.wrap);
    glTexImage2D(GL_TEXTURE_2D, 0, attachment.internalFormat, size[0], size[1], 0, attachment.format, attachment.type, NULL);
    CHECK_GL(FATAL);
    const auto ptr = res::Pointer(res::Tag(res::kTextureType), (uword) texture);
    return TextureRef(ptr);
  }

  FrameBuffer::FrameBuffer(const Dimension& size, const std::vector<ColorAttachment>& color_attachments):
    fbo_(true, true, false),
    mesh_(kFrameBufferMesh),
    color_buffers_(),
    dbuff_(size),
    size_(size) {
    const auto num_attachments = color_attachments.empty() ? 1 : color_attachments.size();
    DLOG(INFO) << "creating FrameBuffer w/ " << num_attachments << " color attachments.";
    unsigned int attachments[num_attachments];
    if(color_attachments.empty()) {
      const auto buffer = CreateColorBuffer(size, ColorAttachment {
        .slot = GL_COLOR_ATTACHMENT0,
        .internalFormat = GL_RGBA16F,
        .format = GL_RGBA,
        .type = GL_FLOAT,
        .alignment = texture::kDefaultAlignment,
        .filter = texture::kDefaultFilter,
        .wrap = texture::kDefaultWrap,
      });
      attachments[0] = GL_COLOR_ATTACHMENT0;
      color_buffers_.push_back(buffer);
      fbo_.Attach(0, buffer);
    } else {
      for(auto idx = 0; idx < color_attachments.size(); idx++) {
        DLOG(INFO) << "attaching color buffer #" << idx;
        const auto& attachment = color_attachments[idx];
        const auto color_buffer = CreateColorBuffer(size, attachment);
        color_buffers_.push_back(color_buffer);
        fbo_.Attach(static_cast<FrameBufferTextureAttachment>(attachment.slot), color_buffer);
        attachments[idx] = attachment.slot;
      }
    }
    glDrawBuffers(num_attachments, attachments);
    CHECK_GL(FATAL);
    fbo_.Attach(dbuff_);
    fbo_.CheckStatus<google::FATAL>();
    fbo_.Unbind();
  }

  void FrameBuffer::OnPreInit() {
  }

  void FrameBuffer::OnInit() {
    kFrameBufferMesh = d2::NewMesh(kFrameBufferVertices);
  }

  void FrameBuffer::OnPostInit() {
  }

  void FrameBuffer::Init() {
    Engine::OnPreInit(&OnPreInit);
    Engine::OnInit(&OnInit);
    Engine::OnPostInit(&OnPostInit);
  }

  FrameBuffer* FrameBuffer::New(const Dimension& size, const std::vector<ColorAttachment>& color_attachments) {
    return new FrameBuffer(size, color_attachments);
  }
}