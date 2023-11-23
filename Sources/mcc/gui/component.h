#ifndef MCC_GUI_COMPONENT_H
#define MCC_GUI_COMPONENT_H

#include "mcc/gui/constants.h"

namespace mcc::gui {
  class Component {
    friend class Container;
    DEFINE_NON_COPYABLE_TYPE(Component);
  protected:
    Position pos_;
    Dimension size_;

    Component():
      pos_(0, 0),
      size_(0, 0) {
    }

    void SetSize(const Dimension& size) {
      size_ = size;
    }

    void SetPos(const Position& pos) {
      pos_ = pos;
    }

    virtual void Render() = 0;
  public:
    virtual ~Component() = default;
    
    Position GetPosition() const {
      return pos_;
    }

    Dimension GetSize() const {
      return size_;
    }
  };
}

#endif //MCC_GUI_COMPONENT_H