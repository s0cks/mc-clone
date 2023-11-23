#ifndef MCC_GUI_COMPONENT_H
#define MCC_GUI_COMPONENT_H

namespace mcc::gui {
  class Component {
  protected:
    Component() = default;
  public:
    virtual ~Component() = default;
  };
}

#endif //MCC_GUI_COMPONENT_H