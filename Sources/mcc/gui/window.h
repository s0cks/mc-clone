#ifndef MCC_GUI_WINDOW_H
#define MCC_GUI_WINDOW_H

#include "mcc/gui/container.h"

namespace mcc::gui {
  class Window : public Container {
  protected:
    Window() = default;

    void Render();
  public:
    ~Window() override = default;
  public:
    static inline Window*
    New(const Position& pos, const Dimension& size) {
      auto window = new Window();
      window->SetPos(pos);
      window->SetSize(size);
      return window;
    }
  };
}

#endif //MCC_GUI_WINDOW_H