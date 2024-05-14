#include "mcc/render/render_pass_root.h"

#include "mcc/render/renderer.h"
#include "mcc/render/render_pass_guis.h"
#include "mcc/render/render_pass_text.h"
#include "mcc/render/render_pass_skybox.h"
#include "mcc/render/render_pass_terrain.h"


namespace mcc::render {
  RootRenderPass::RootRenderPass():
    OrderedSequenceRenderPass(),
    terrain_(new TerrainRenderPass()),
    guis_(new RenderPassGuis()),
    text_(new TextRenderPass()),
    // skybox_(new SkyboxRenderPass()),
    clear_(kGreen) {
    Append(terrain_);
    Append(guis_);
    Append(text_);
    // Append(skybox_);
  }

  RootRenderPass::~RootRenderPass() {
    delete terrain_;
    delete guis_;
    delete skybox_;
  }

  void RootRenderPass::Render() {
    const auto renderer = Renderer::Get();
    glPolygonMode(GL_FRONT_AND_BACK, renderer->GetMode());
    CHECK_GL(FATAL);
    glClear(GL_COLOR_BUFFER_BIT);
    CHECK_GL(FATAL);
    gfx::SetClearColor(clear_);
  }
}