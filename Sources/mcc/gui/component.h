#ifndef MCC_GUI_COMPONENT_H
#define MCC_GUI_COMPONENT_H

#include "mcc/gui/constants.h"

namespace mcc::gui {
  class Component {
    friend class Container;
    DEFINE_NON_COPYABLE_TYPE(Component);
  protected:
    Rect bounds_;
    Dimension size_;
    glm::vec2 pos_;

    Component():
      bounds_(0.0f) {
    }

    inline void SetMinX(const float value) {
      bounds_[0] = value;
    }

    inline void SetMinY(const float value) {
      bounds_[1] = value;
    }

    inline void SetMaxX(const float value) {
      bounds_[2] = value;
    }

    inline void SetMaxY(const float value) {
      bounds_[3] = value;
    }

    inline void UpdateBounds() {
      SetMinX(pos_[0]);
      SetMinY(pos_[1]);
      SetMaxX(pos_[0] + size_[0]);
      SetMaxY(pos_[1] + size_[1]);
    }

    virtual void Render() = 0;
  public:
    virtual ~Component() = default;

    inline Rect GetBounds() const {
      return bounds_;
    }

    inline void SetSize(const Dimension& size) {
      size_ = size;
      UpdateBounds();
    }

    inline void SetPosition(const glm::vec2& pos) {
      pos_ = pos;
      UpdateBounds();
    }
    
    Position GetPosition() const {
      return pos_;
    }

    Dimension GetSize() const {
      return size_;
    }
  };
}

#endif //MCC_GUI_COMPONENT_H