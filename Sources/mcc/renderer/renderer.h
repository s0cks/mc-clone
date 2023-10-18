#ifndef MCC_RENDERER_H
#define MCC_RENDERER_H

#include "mcc/common.h"

namespace mcc {
  class Camera;
  class OrthoCamera;
  class PerspectiveCamera;

  class Window;

  class Renderer {
  public:
    enum RenderPass {
      k2D,
      k3D,
    };
  private:
    static void Prepare2d(Window* window, OrthoCamera* camera);
    static void Prepare3d(Camera* camera);
  public:
    Renderer() = delete;
    ~Renderer() = delete;
    DEFINE_NON_COPYABLE_TYPE(Renderer);

    void Prepare(Window* window, Camera* camera);
  };
}

#endif //MCC_RENDERER_H