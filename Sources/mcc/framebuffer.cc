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
  static VertexArrayObject vao_(kInvalidVertexArrayObject);
  static shader::Shader shader_;

  FrameBuffer::FrameBuffer(const uint64_t width, 
                           const uint64_t height):
    width_(width),
    height_(height),
    fbo_(),
    tex_(width, height),
    vbo_(kFrameBufferVertices) {
  }

  void FrameBuffer::OnPreInit() {

  }

  void FrameBuffer::OnInit() {
    shader_ = shader::Shader::Get("framebuffer");
    vao_ = VertexArrayObject();
  }

  void FrameBuffer::OnPostInit() {
  }

  void FrameBuffer::Init() {
    Engine::OnPreInit(&OnPreInit);
    Engine::OnInit(&OnInit);
    Engine::OnPostInit(&OnPostInit);
  }

  VertexArrayObject FrameBuffer::GetVao() {
    return vao_;
  }

  FrameBuffer* FrameBuffer::New(const uint64_t width, const uint64_t height) {
    VertexArrayObjectScope vao(vao_);
    return new FrameBuffer(width, height);
  }

  void FrameBuffer::Draw() {
    shader_.ApplyShader();
    shader_.SetInt("tex0", 0);
    fbo_.Unbind();
    shader_.ApplyShader();
    VertexArrayObjectScope vao(vao_);
    vbo_.Bind();
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(FrameBufferVertex), (const GLvoid*) 0);
    CHECK_GL(FATAL);
    glEnableVertexAttribArray(0);
    CHECK_GL(FATAL);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(FrameBufferVertex), (const GLvoid*) offsetof(FrameBufferVertex, uv));
    CHECK_GL(FATAL);
    glEnableVertexAttribArray(1);
    CHECK_GL(FATAL);
    glDisable(GL_DEPTH_TEST);
    CHECK_GL(FATAL);
    tex_.Bind();
    glDrawArrays(GL_TRIANGLES, 0, vbo_.length());
    CHECK_GL(FATAL);
    vbo_.Unbind();
    tex_.Unbind();
    glEnable(GL_DEPTH_TEST);
  }
}