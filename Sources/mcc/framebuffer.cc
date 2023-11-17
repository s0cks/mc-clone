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

  static inline TextureRef
  CreateColorBuffer(const Dimension& size) {
    const auto alignment = texture::PixelStoreAlignment{};
    const auto filter = texture::TextureFilter(texture::kLinear);
    const auto texture = new texture::Texture(texture::k2D, true, true, false, alignment, filter);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, size[0], size[1], 0, GL_RGBA, GL_FLOAT, NULL);
    CHECK_GL(FATAL);
    const auto ptr = res::Pointer(res::Tag(res::kTextureType), (uword) texture);
    return TextureRef(ptr);
  }

  FrameBuffer::FrameBuffer(const Dimension& size):
    fbo_(true, true, false),
    mesh_(d2::NewMesh(kFrameBufferVertices)),
    cbuff_(CreateColorBuffer(size)),
    dbuff_(size),
    size_(size) {
    fbo_.Attach(kColorAttachment0, cbuff_);
    fbo_.Attach(dbuff_);
    fbo_.CheckStatus<google::FATAL>();
    fbo_.Unbind();
  }

  void FrameBuffer::OnPreInit() {
  }

  void FrameBuffer::OnInit() {
  }

  void FrameBuffer::OnPostInit() {
  }

  void FrameBuffer::Init() {
    Engine::OnPreInit(&OnPreInit);
    Engine::OnInit(&OnInit);
    Engine::OnPostInit(&OnPostInit);
  }

  FrameBuffer* FrameBuffer::New(const Dimension& size) {
    return new FrameBuffer(size);
  }
}