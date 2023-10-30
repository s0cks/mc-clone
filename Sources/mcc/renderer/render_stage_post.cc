#include "mcc/renderer/renderer_stage.h"
#include "mcc/renderer/renderer.h"
#include "mcc/window.h"

namespace mcc::renderer {
  void PostRenderStage::OnCheck(uv_check_t* handle) {
    const auto window = Window::GetHandle();
    const auto stage = GetHandleStage<PostRenderStage>(handle);
    Renderer::SetState(stage->GetState());
    DLOG(INFO) << "post-render.";

    glfwSwapBuffers(window);
    CHECK_GL(FATAL);

    Renderer::ResetEntityCounter();
  }
}