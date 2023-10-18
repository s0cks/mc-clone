#ifndef MCC_ENGINE_H
#define MCC_ENGINE_H

#include "mcc/common.h"

namespace mcc {
  class Camera;
  class Renderer;
  class Window;
  class Engine {
  private:
    static void SetWindow(Window* window);
  public:
    DEFINE_NON_INSTANTIABLE_TYPE(Engine);
    static void Initialize();
    static Window* GetWindow();
  };
}

#endif //MCC_ENGINE_H