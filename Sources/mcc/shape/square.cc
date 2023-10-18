#include "mcc/shape/square.h"
#include "mcc/camera/camera.h"

namespace mcc {
  void Square::Render() {
    shader_.ApplyShader();
    mesh_->Render();
  }
}