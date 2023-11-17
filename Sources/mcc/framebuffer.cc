#include "mcc/framebuffer.h"
#include "mcc/engine/engine.h"
#include "mcc/shader/shader.h"

namespace mcc {
  static const FrameBufferVertexList kFrameBufferVertices = {
    { .pos = glm::vec2(1.0f, -1.0f), .uv = glm::vec2(1.0f, 0.0f) },
    { .pos = glm::vec2(-1.0f, -1.0f), .uv = glm::vec2(0.0f, 0.0f) },
    { .pos = glm::vec2(-1.0f, 1.0f), .uv = glm::vec2(0.0f, 1.0f) },

    { .pos = glm::vec2(1.0f, 1.0f), .uv = glm::vec2(1.0f, 1.0f) },
    { .pos = glm::vec2(1.0f, -1.0f), .uv = glm::vec2(1.0f, 0.0f) },
    { .pos = glm::vec2(-1.0f, 1.0f), .uv = glm::vec2(0.0f, 1.0f) },
  };
  static VertexArrayObject kFrameBufferVao(kInvalidVertexArrayObject);
  static ShaderRef kFrameBufferShader;

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
    vao_(kFrameBufferVao),
    vbo_(kFrameBufferVertices),
    fbo_(true, true, false),
    cbuff_(CreateColorBuffer(size)),
    dbuff_(size),
    shader_(kFrameBufferShader),
    size_(size) {
    fbo_.Attach(kColorAttachment0, cbuff_);
    fbo_.Attach(dbuff_);
    fbo_.CheckStatus<google::FATAL>();
    fbo_.Unbind();
  }

  void FrameBuffer::OnPreInit() {

  }

  void FrameBuffer::OnInit() {
    kFrameBufferShader = GetShader("framebuffer");
    kFrameBufferVao = VertexArrayObject();
  }

  void FrameBuffer::OnPostInit() {
  }

  void FrameBuffer::Init() {
    Engine::OnPreInit(&OnPreInit);
    Engine::OnInit(&OnInit);
    Engine::OnPostInit(&OnPostInit);
  }

  FrameBuffer* FrameBuffer::New(const Dimension& size) {
    VertexArrayObjectScope vao_scope(kFrameBufferVao);
    return new FrameBuffer(size);
  }

  void FrameBuffer::Draw() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    CHECK_GL(FATAL);
    TextureBindScope<0> tex(cbuff_);
    InvertedDepthTestScope depth_test;
    shader_->ApplyShader();
    shader_->SetInt("tex", 0);
    shader_->SetBool("hdr", true);
    shader_->SetFloat("gamma", 2.2f);
    shader_->SetFloat("exposure", 1.0f);
    VertexArrayObjectScope vao(vao_);
    glDrawArrays(GL_TRIANGLES, 0, vbo_.length());
    CHECK_GL(FATAL);
  }
}