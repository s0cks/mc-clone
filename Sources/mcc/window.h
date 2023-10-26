#ifndef MCC_WINDOW_H
#define MCC_WINDOW_H

#include "mcc/gfx.h"
#include "mcc/common.h"

namespace mcc {
  class Window {
    DEFINE_NON_INSTANTIABLE_TYPE(Window);
  private:
    static void OnPreInit();
    static void OnInit();
    static void OnPostInit();
    static void OnTerminating();
    static void OnTerminated();
    static void OnWindowClosed(GLFWwindow* window);
  public:
    static void Init();
    static void Close();
    static GLFWwindow* GetHandle();
    static glm::vec2 GetSize();
    static glm::vec2 GetInitSize();
  };
}

#endif //MCC_WINDOW_H