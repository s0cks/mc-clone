#include "mcc/shape/square.h"
#include "mcc/camera/camera.h"

namespace mcc {
  void Square::Render() {
    shader_.ApplyShader();
    shader_.SetMat4("projection", OrthoCamera::GetCurrentProjectionMatrix());
    shader_.ApplyShader();
    mesh_->Render();
  }
}