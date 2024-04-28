#ifndef MCC_GUI_WINDOW_H
#define MCC_GUI_WINDOW_H

#include "mcc/color.h"
#include "mcc/gui/gui_events.h"
#include "mcc/gui/gui_component.h"

namespace mcc {
  namespace gui {
    class Window : public ContainerComponent {
    protected:
      Color bg_;

      Window();

      bool Accept(ComponentVisitor* vis) override {
        return vis->VisitWindow(this);
      }

      void SetBackground(const Color& color) {
        bg_ = color;
      }
    public:
      ~Window();
      DECLARE_GUI_COMPONENT_TYPE(Window);

      const Color& GetBackground() const {
        return bg_;
      }

      inline rx::observable<WindowOpenedEvent*> OnOpened() const {
        return OnEvent()
          .filter(WindowOpenedEvent::Filter)
          .map(WindowOpenedEvent::Cast);
      }

      inline rx::observable<WindowClosedEvent*> OnClosed() const {
        return OnEvent()
          .filter(WindowClosedEvent::Filter)
          .map(WindowClosedEvent::Cast);
      }
    public:
      static Window* New() {
        return new Window();
      }
    };
  }
}

#endif //MCC_GUI_WINDOW_H