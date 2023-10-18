#include "mcc/renderer/renderer.h"
#include "mcc/camera/camera.h"
#include "mcc/window.h"

namespace mcc {
  void Renderer::Prepare2d(Window* window, OrthoCamera* camera) {
    glClear(GL_DEPTH_BUFFER_BIT);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  }
}