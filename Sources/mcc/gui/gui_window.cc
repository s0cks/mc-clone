#include "mcc/gui/gui_window.h"

#include "mcc/gui/gui_tree.h"

namespace mcc::gui {
  Window::Window():
    ContainerComponent() {
    Tree::AddRoot(this);
  }

  Window::~Window() {
    Tree::RemoveRoot(this); 
  }
}