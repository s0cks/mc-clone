
#include "mcc/render/render_pass_text.h"

#include "mcc/thread_local.h"
#include "mcc/vao/vao.h"
#include "mcc/vao/vao_scope.h"
#include "mcc/vbo/vbo.h"
#include "mcc/vbo/vbo_scope.h"
#include "mcc/vbo/vbo_builder.h"
#include "mcc/vbo/vbo_attribute.h"
#include "mcc/ibo/ibo.h"
#include "mcc/program/program_scope.h"
#include "mcc/cull_face_scope.h"
#include "mcc/camera/camera_ortho.h"

#include "mcc/text/text.h"

#include "mcc/font/font.h"
#include "mcc/font/font_renderer.h"

#include "mcc/window/window.h"

namespace mcc::render {
  bool TextRenderPass::ShouldSkip() const {
    const auto& all = text::GetAll();
    return !all.empty();
  }

  static ThreadLocal<Vao> vao_;
  static ThreadLocal<Vbo> vbo_;
  static ThreadLocal<UIntIbo> ibo_;

  static inline Vao*
  SetVao(Vao* vao) {
    MCC_ASSERT(vao);
    MCC_ASSERT(!vao_);
    vao_.Set(vao);
    return vao;
  }

  static inline Vao*
  CreateVao() {
    DLOG(INFO) << "creating gui vao....";
    const auto vao = Vao::New();
    DLOG(INFO) << "gui vao: " << vao->ToString();
    return vao;
  }

  static inline Vao*
  GetVao() {
    return vao_.Get();
  }

  static inline bool
  HasVao() {
    return (bool) vao_;
  }

  static inline Vao*
  GetOrCreateVao() {
    if(HasVao())
      return GetVao();
    return SetVao(CreateVao());
  }

  static inline bool
  HasVbo() {
    return (bool) vbo_;
  }

  static inline Vbo*
  SetVbo(Vbo* vbo) {
    MCC_ASSERT(vbo);
    MCC_ASSERT(!vbo_);
    vbo_.Set(vbo);
    return vbo;
  }

  static inline Vbo*
  GetVbo() {
    return vbo_.Get();
  }

  struct Vertex {
    glm::vec2 pos;
  };
  typedef std::vector<Vertex> VertexList;

  class PosAttr : public vbo::Vec2fAttribute<0, sizeof(Vertex), offsetof(Vertex, pos)>{};

  static inline Vbo*
  CreateVbo(const int32_t num_vertices) {
    MCC_ASSERT(num_vertices >= 1);
    DLOG(INFO) << "creating gui vbo w/ " << num_vertices << " vertices....";
    vbo::VboBuilder<Vertex,
                    PosAttr> builder(num_vertices, gfx::kDynamicDrawUsage);
    const auto vbo = builder.Build()
      .as_blocking()
      .first();
    DLOG(INFO) << "gui vbo: " << vbo->ToString();
    return vbo;
  }

  static inline Vbo*
  GetOrCreateVbo(const int32_t num_vertices) {
    if(HasVbo())
      return GetVbo();
    return SetVbo(CreateVbo(num_vertices));
  }

  static inline bool
  HasIbo() {
    return (bool) ibo_;
  }

  static inline UIntIbo*
  SetIbo(UIntIbo* ibo) {
    MCC_ASSERT(ibo);
    MCC_ASSERT(!ibo_);
    ibo_.Set(ibo);
    return ibo;
  }

  static inline UIntIbo*
  CreateIbo(const int32_t num_indices) {
    DLOG(INFO) << "creating gui ibo w/ " << num_indices << " indices....";
    const auto ibo = UIntIbo::New(num_indices);
    DLOG(INFO) << "gui ibo: " << ibo->ToString();
    return ibo;
  }

  static inline UIntIbo*
  GetIbo() {
    return ibo_.Get();
  }

  static inline UIntIbo*
  GetOrCreateIbo(const int32_t num_indices) {
    if(HasIbo())
      return GetIbo();
    return SetIbo(CreateIbo(num_indices));
  }

  using font::Font;
  static ThreadLocal<Font> font_;

  static inline Font*
  GetFont() {
    if(font_)
      return font_.Get();
    const auto font = new Font("arial/arial");
    font_.Set(font);
    return font;
  }

  TextRenderPass::TextRenderPass():
    RenderPass(),
    program_(Program::FromJson("program:text")) {
  }

  void TextRenderPass::Render() {
    // compute vertices && indices for roots
    // VertexList vertices;
    // UIntIbo::IndexList indices;

    // const auto vao = GetOrCreateVao();
    // vao::VaoBindScope scope(vao);
    // const auto vbo = GetOrCreateVbo(128);
    // {
    //   vbo::VboUpdateScope update(vbo);
    //   update.Update(vertices);
    // }

    // InvertedCullFaceScope cull_face;
    // InvertedDepthTestScope depth_test;
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    CHECK_GL(FATAL);
    glClearColor(0.1f, 0.1f, 0.1f, 0.1f);
    CHECK_GL(FATAL);

    const auto window = Window::Get();
    MCC_ASSERT(window);
    const auto size = window->GetSize();
    const auto projection = glm::ortho(0.0f, size[0] * 1.0f, 0.0f, size[1] * 1.0f, 0.1f, 1000.0f);
    font::FontRenderer renderer(projection, GetFont());
    renderer.RenderText("Hello World", glm::vec2(25.0f, 25.0f));

    // if(vertices.empty()) {
    //   LOG(WARNING) << "no vertices to render.";
    //   return;
    // }

    // auto model = glm::mat4(1.0f);
    // vbo::VboDrawScope draw_scope(vbo);
    // program::ApplyProgramScope prog(program_);
    // prog.Set("projection", projection);
    // prog.Set("model", model);
    // draw_scope.DrawTriangles();
  } 
}