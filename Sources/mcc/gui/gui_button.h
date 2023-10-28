#ifndef MCC_GUI_BUTTON_H
#define MCC_GUI_BUTTON_H

#include <string>
#include "mcc/gui/gui_component.h"

namespace mcc::gui {
  class Button : public Component {
  private:
    std::string text_;
  public:
    Button(const std::string& text):
      Component(),
      text_(text) {
    }
    ~Button() override = default;

    std::string GetText() const {
      return text_;
    }

    void Render() override;
  };
}

#endif //MCC_GUI_BUTTON_H