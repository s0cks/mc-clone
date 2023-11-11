#include "mcc/renderer/stage.h"
#include "mcc/window.h"
#include "mcc/gui/gui.h"
#include "mcc/renderer/renderer.h"

namespace mcc::renderer {
  void PreRenderStage::OnPrepare(uv_idle_t* handle) {
    const auto stage = GetHandleStage<PreRenderStage>(handle);
    Renderer::SetState(stage->GetState());
    VLOG(3) << "pre-render.";

    auto fb = Renderer::GetFrameBuffer();
    fb->Bind();

    const auto window = Window::GetHandle();
    int width;
    int height;
    glfwGetFramebufferSize(window, &width, &height);
    CHECK_GL(FATAL);
    glViewport(0, 0, width, height);
    CHECK_GL(FATAL);
    glfwPollEvents();
    CHECK_GL(FATAL);

    gui::Screen::NewFrame();

    glPolygonMode(GL_FRONT_AND_BACK, Renderer::GetMode());
    CHECK_GL(FATAL);
    glClearColor(0.4, 0.3, 0.4, 1.0f);
    CHECK_GL(FATAL);
    glEnable(GL_DEPTH_TEST);
    CHECK_GL(FATAL);
    glDepthFunc(GL_LEQUAL);
    CHECK_GL(FATAL);
    glEnable(GL_CULL_FACE);
    CHECK_GL(FATAL);
    glFrontFace(GL_CW);
    CHECK_GL(FATAL);
    glCullFace(GL_BACK);
    CHECK_GL(FATAL);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    CHECK_GL(FATAL);
  }
}