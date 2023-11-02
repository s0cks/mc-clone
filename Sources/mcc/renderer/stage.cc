#include "mcc/renderer/stage.h"
#include "mcc/renderer/renderer.h"
#include "mcc/engine/engine.h"
#include "mcc/camera/perspective_camera.h"

#include <glog/logging.h>

namespace mcc::renderer {
  void RenderStage::OnPrepare(uv_prepare_t* handle) {
    const auto stage = GetHandleStage<RenderStage>(handle);
    const auto tick = Engine::GetTick();

    const auto camera = camera::PerspectiveCameraBehavior::GetCameraComponent();
    LOG_IF(FATAL, !camera) << "no camera component found.";

    const auto projection = (*camera)->GetProjectionMatrix();
    const auto view = (*camera)->GetViewMatrix();
    Renderer::SetState(stage->GetState());
    stage->Render(tick, projection, view);
  }
}