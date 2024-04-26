
#include "mcc/renderer/render_pass_guis.h"

#include "mcc/shape.h"
#include "mcc/gui/gui_tree.h"
#include "mcc/gui/gui_window.h"
#include "mcc/gui/gui_context.h"

#include "mcc/thread_local.h"
#include "mcc/vao/vao_scope.h"

#include "mcc/vbo/vbo_builder.h"

namespace mcc::render {
  using namespace gui;

  bool RenderPassGuis::ShouldSkip() const {
    return Tree::IsEmpty();
  }

  class ComponentRenderer : public gui::ComponentVisitor {
  protected:
    gui::VertexList& vertices_;
    UIntIbo::IndexList& indices_;
  public:
    ComponentRenderer(gui::VertexList& vertices,
                      UIntIbo::IndexList& indices):
      ComponentVisitor(),
      vertices_(vertices),
      indices_(indices) {
    }
    ~ComponentRenderer() override = default;

    bool VisitWindow(gui::Window* window) override {
      shape::NewCenteredRect(vertices_, indices_, window->GetPos(), window->GetSize());
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
                    gui::PosAttr, gui::ColorAttr> builder(num_vertices, vbo::kDynamicDraw);
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

  void RenderPassGuis::Render() {
    // compute vertices && indices for roots
    gui::VertexList vertices;
    UIntIbo::IndexList indices;
    ComponentRenderer renderer(vertices, indices);
    const auto num_roots = Tree::GetNumberOfRoots();
    DLOG(INFO) << "rendering " << num_roots << " root components.....";
    for(auto idx = 0; idx < num_roots; idx++) {
      const auto root = Tree::GetRoot(idx);
      if(!root) {
        DLOG(WARNING) << "root #" << idx << " is null.";
        continue;
      }

      LOG_IF(ERROR, !root->Accept(&renderer)) << "failed to render root: " << root;
    }

    // update vao, vbo, ibo data on gpu
    const auto vao = GetOrCreateVao();
    vao::VaoBindScope scope(vao);
    const auto vbo = GetOrCreateVbo(512);
    const auto ibo = GetOrCreateIbo(512);

    // draw
    // disable depth test & cull face
    // bind vao
    // bind & enable vbo
    // bind ibo
    // draw all root components
  } 
}