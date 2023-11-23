#ifndef MCC_GUI_CONTAINER_H
#define MCC_GUI_CONTAINER_H

#include <vector>
#include "mcc/common.h"
#include "mcc/gui/component.h"

namespace mcc::gui {
  typedef std::vector<Component*> ComponentList;
  class Container : public Component {
    DEFINE_NON_COPYABLE_TYPE(Container);
  protected:
    ComponentList children_;

    Container():
      Component(),
      children_() {
    }
    explicit Container(const ComponentList& children):
      Component(),
      children_(children) {
    }

    void AddChild(Component* child) {
      children_.push_back(child);
    }

    void RenderChildren() {
      for(const auto& child : children_)
        child->Render();
    }
  public:
    ~Container() override = default;

    uint64_t GetNumberOfChildren() const {
      return children_.size();
    }

    Component* GetChildAt(const uint64_t idx) const {
      return children_[idx];
    }
  };
}

#endif //MCC_GUI_CONTAINER_H