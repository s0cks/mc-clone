#include "mcc/renderer/render_pass_guis.h"

#include "mcc/gui/gui_tree.h"

namespace mcc::render {
  using namespace gui;

  bool RenderPassGuis::ShouldSkip() const {
    return Tree::IsEmpty();
  }

  void RenderPassGuis::Render() {
    MCC_ASSERT(Tree::HasRoots());
    // compute vertices && indices for roots
    const auto num_roots = Tree::GetNumberOfRoots();
    DLOG(INFO) << "rendering " << num_roots << " root components.....";
    for(auto idx = 0; idx < num_roots; idx++) {
      const auto root = Tree::GetRoot(idx);
      if(!root) {
        DLOG(WARNING) << "root #" << idx << " is null.";
        continue;
      }
    }

    // update vao, vbo, ibo data on gpu

    // draw
    // disable depth test & cull face
    // bind vao
    // bind & enable vbo
    // bind ibo
    // draw all root components
  } 
}