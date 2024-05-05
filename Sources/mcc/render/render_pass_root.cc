#include "mcc/render/render_pass_root.h"

#include "mcc/window/window.h"
#include "mcc/render/renderer.h"
#include "mcc/render/render_pass_guis.h"
#include "mcc/render/render_pass_terrain.h"


namespace mcc::render {
  RootRenderPass::RootRenderPass():
    OrderedSequenceRenderPass(),
    terrain_(new TerrainRenderPass()),
    guis_(new RenderPassGuis()) {
    Append(terrain_);
    Append(guis_);
  }

  RootRenderPass::~RootRenderPass() {
    delete terrain_;
    delete guis_;
  }

  void RootRenderPass::Render() {
    const auto window = Window::Get();
    const auto renderer = Renderer::Get();

    glfwPollEvents();
    CHECK_GL(FATAL);
    //TODO: remove chunk
    glPolygonMode(GL_FRONT_AND_BACK, renderer->GetMode());
    CHECK_GL(FATAL);
    glEnable(GL_DEPTH_TEST);
    CHECK_GL(FATAL);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    CHECK_GL(FATAL);
    glDepthFunc(GL_LEQUAL);
    CHECK_GL(FATAL);
    glClearColor(0.9f, 0.9f, 0.9f, 1.0f);
    CHECK_GL(FATAL);
  }
}