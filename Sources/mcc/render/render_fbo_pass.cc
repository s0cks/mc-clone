#include "mcc/render/render_fbo_pass.h"

#include "mcc/shape.h"
#include "mcc/vao/vao.h"
#include "mcc/vbo/vbo.h"
#include "mcc/ibo/ibo.h"
#include "mcc/thread_local.h"
#include "mcc/vao/vao_scope.h"
#include "mcc/vbo/vbo_scope.h"
#include "mcc/window/window.h"
#include "mcc/gui/gui_context.h"
#include "mcc/vbo/vbo_builder.h"
#include "mcc/cull_face_scope.h"
#include "mcc/camera/camera_ortho.h"
#include "mcc/render/render_settings.h"

namespace mcc::render {
  struct Vertex {
    glm::vec3 pos;
    glm::vec2 uv;
    Color color;
    
    Vertex() = default;
    Vertex(const Vertex& rhs) = default;
    ~Vertex() = default;

    Vertex& operator=(const Vertex& rhs) = default;

    friend std::ostream& operator<<(std::ostream& stream, const Vertex& rhs) {
      stream << "gui::Vertex(";
      stream << "pos=" << glm::to_string(rhs.pos) << ", ";
      stream << "uv=" << glm::to_string(rhs.uv) << ", ";
      stream << "color=" << glm::to_string(rhs.color);
      stream << ")";
      return stream;
    }
  };

  typedef std::vector<Vertex> VertexList;

  class PosAttr : public vbo::Vec3fAttribute<0, sizeof(Vertex), offsetof(Vertex, pos)>{};
  class UvAttr : public vbo::Vec2fAttribute<1, sizeof(Vertex), offsetof(Vertex, uv)>{};
  class ColorAttr : public vbo::ColorAttribute<2, sizeof(Vertex), offsetof(Vertex, color)>{};

  static ThreadLocal<Vao> vao_;
  static ThreadLocal<Vbo> vbo_;

  static Vao* GetVao() {
    if(vao_)
      return vao_.Get();
    const auto vao = Vao::New();
    vao_.Set(vao);
    return vao;
  }

  static Vbo* GetVbo() {
    if(vbo_)
      return vbo_.Get();
    VertexList vertices;
    UIntIbo::IndexList indices;
    shape::NewTexturedCenteredRect(vertices, indices, glm::vec2(0.0f), glm::vec2(1.0f), kBlack);
    vbo::VboBuilder<Vertex,
                    PosAttr, UvAttr, ColorAttr> builder(vertices, vbo::kStaticDraw);
    const auto vbo = builder.Build()
      .as_blocking()
      .first();
    vbo_.Set(vbo);
    return vbo;
  }

  void RenderFboPass::Render() {
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_->GetId());
    CHECK_GL(FATAL);

    DLOG(INFO) << "rendering " << fbo_->ToString() << "....";
    const auto vao = GetVao();
    vao::VaoBindScope vao_scope(vao);
    const auto vbo = GetVbo();

    InvertedCullFaceScope cull_face;
    InvertedDepthTestScope depth_test;
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    CHECK_GL(FATAL);
    
    const auto attachments = fbo_->GetAttachments();
    const auto attachment = attachments.GetAllColorAttachments()
      .as_blocking()
      .first()->AsColorAttachment();
    MCC_ASSERT(attachment);
    const auto texture = attachment->GetTexture();
    texture->Bind0();

    fbo::FboBindScope fbo(fbo_);
    const auto window = Window::Get();
    MCC_ASSERT(window);
    
    const auto projection = glm::ortho(-0.5f, 0.5f, -0.5f, 0.5f, 0.1f, 1000.0f);
    const auto view = glm::lookAt(glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    auto model = glm::mat4(1.0f);
    vbo::VboDrawScope draw_scope(vbo);
    program_->Apply();
    program_->SetInt("tex", 0);
    program_->SetMat4("projection", projection);
    program_->SetMat4("view", view);
    program_->SetMat4("model", model);
    program_->Apply();
    draw_scope.DrawTriangles();
  }
}