#ifndef MCC_GUI_COMPONENT_H
#define MCC_GUI_COMPONENT_H

#include <vector>
#include "mcc/gfx.h"

namespace mcc::gui {
  class Component {
  protected:
    Component() = default;
  public:
    virtual ~Component() = default;
    virtual void Render() = 0;
  };

  class ContainerComponent : public Component {
  protected:
    std::vector<Component*> children_;

    ContainerComponent():
      Component(),
      children_() {
    }

    void AddChild(Component* component) {
      children_.push_back(component);
    }
  public:
    ~ContainerComponent() override = default;

    void Render() override {
      for(auto& child : children_)
        child->Render();
    }
  };
}

#endif //MCC_GUI_COMPONENT_H