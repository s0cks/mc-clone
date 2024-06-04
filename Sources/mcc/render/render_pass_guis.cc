
#include "mcc/render/render_pass_guis.h"

#include "mcc/shape.h"
#include "mcc/gui/gui_tree.h"
#include "mcc/gui/gui_window.h"
#include "mcc/gui/gui_context.h"

#include "mcc/thread_local.h"
#include "mcc/vao/vao_scope.h"

#include "mcc/ibo/ibo_scope.h"
#include "mcc/vbo/vbo_scope.h"
#include "mcc/vbo/vbo_builder.h"

#include "mcc/window/window.h"

#include "mcc/cull_face_scope.h"

#include "mcc/render/renderer.h"
#include "mcc/render/render_fbo_pass.h"

#include "mcc/program/program_scope.h"

namespace mcc::render {
  bool RenderPassGuis::ShouldSkip() const {
    return gui::Tree::IsEmpty();
  }

  class ComponentRenderer : public gui::ComponentVisitor {
  protected:
    gui::VertexList& vertices_;
    UIntIbo::IndexList& indices_;
    glm::vec2 fb_size_;
  public:
    ComponentRenderer(gui::VertexList& vertices,
                      UIntIbo::IndexList& indices, 
                      const glm::vec2& fb_size):
      ComponentVisitor(),
      vertices_(vertices),
      indices_(indices),
      fb_size_(fb_size) {
    }
    ~ComponentRenderer() override = default;

    bool VisitWindow(gui::Window* window) override {
      const auto win = Window::Get();
      MCC_ASSERT(win);
      const auto fb_size = win->GetSize();
      DLOG(INFO) << "fb_size: " << glm::to_string(fb_size);
      const auto size = window->GetSize();
      DLOG(INFO) << "size: " << glm::to_string(size);
      const float xScale = size[0] / static_cast<float>(fb_size[0]);
      const float yScale = size[1] / static_cast<float>(fb_size[1]);
      const auto scale = glm::vec2(xScale, yScale);
      DLOG(INFO) << "scale: " << glm::to_string(scale);
      shape::NewRect(vertices_, indices_, window->GetPos(), glm::vec2(scale), window->GetBackground());
      return true;
    }
  };

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

  static inline Vbo*
  CreateVbo(const int32_t num_vertices) {
    MCC_ASSERT(num_vertices >= 1);
    DLOG(INFO) << "creating gui vbo w/ " << num_vertices << " vertices....";
    vbo::VboBuilder<gui::Vertex,
                    gui::PosAttr, gui::UvAttr, gui::ColorAttr> builder(num_vertices, gfx::kDynamicDrawUsage);
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

  RenderPassGuis::RenderPassGuis():
    RenderPass(),
    prog_(Program::FromJson("program:colored_2d")) {
  }

  void RenderPassGuis::Render() {
    const auto window = Window::Get();
    MCC_ASSERT(window);
    const auto fb_size = window->GetSize();

    // compute vertices && indices for roots
    gui::VertexList vertices;
    UIntIbo::IndexList indices;
    ComponentRenderer renderer(vertices, indices, fb_size);
    const auto num_roots = gui::Tree::GetNumberOfRoots();
    DLOG(INFO) << "rendering " << num_roots << " root components.....";
    LOG_IF(ERROR, !gui::Tree::VisitAllRoots(&renderer)) << "failed to render tree.";

    if(vertices.empty()) {
      LOG(WARNING) << "no vertices to render.";
      return;
    }
    const auto vao = GetOrCreateVao();
    vao::VaoBindScope scope(vao);
    const auto vbo = GetOrCreateVbo(128);
    {
      vbo::VboUpdateScope update(vbo);
      update.Update(vertices);
    }

    InvertedCullFaceScope cull_face;
    InvertedDepthTestScope depth_test;
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    CHECK_GL(FATAL);
    glClearColor(0.1f, 0.1f, 0.1f, 0.1f);
    CHECK_GL(FATAL);

    const auto camera = camera::GetOrthoCamera();
    MCC_ASSERT(camera);
    auto model = glm::mat4(1.0f);
    vbo::VboDrawScope draw_scope(vbo);
    program::ApplyProgramScope prog(prog_);
    prog.Set("projection", camera->GetProjection());
    prog.Set("view", camera->GetView());
    prog.Set("model", model);
    draw_scope.DrawTriangles();

    DLOG(INFO) << "camera pos: " << glm::to_string(camera->GetPos());
  }
}