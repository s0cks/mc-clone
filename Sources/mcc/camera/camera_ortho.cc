#include "mcc/camera/camera_ortho.h"

#include "mcc/window/window.h"

namespace mcc {
  OrthoCamera::OrthoCamera(Window* window):
    OrthoCamera(window->GetSize(), window->GetAspectRatio()) {
  }
}