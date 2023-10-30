#include "mcc/renderer/stage.h"
#include "mcc/renderer/renderer.h"
#include "mcc/engine/engine.h"
#include "mcc/camera/perspective_camera.h"

namespace mcc::renderer {
  void RenderStage::OnPrepare(uv_prepare_t* handle) {
    const auto stage = GetHandleStage<RenderStage>(handle);
    const auto tick = Engine::GetTick();
    const auto projection = camera::PerspectiveCameraBehavior::CalculateProjectionMatrix();
    const auto view = camera::PerspectiveCameraBehavior::CalculateViewMatrix();
    Renderer::SetState(stage->GetState());
    stage->Render(tick, projection, view);
  }
}