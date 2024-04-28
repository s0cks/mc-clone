#ifndef MCC_GUI_COMPONENT_H
#define MCC_GUI_COMPONENT_H

#include "mcc/gui/gui_events.h"
#include "mcc/gui/gui_constants.h"

namespace mcc {
  namespace mouse {
    class MouseEvent;
  }
}

namespace mcc::gui {
#define FOR_EACH_GUI_COMPONENT_TYPE(V)          \
  V(Window)

  class Component;
#define FORWARD_DECLARE(Name) class Name;
  FOR_EACH_GUI_COMPONENT_TYPE(FORWARD_DECLARE)
#undef FORWARD_DECLARE

  class ComponentVisitor {
  protected:
    ComponentVisitor() = default;
  public:
    virtual ~ComponentVisitor() = default;
#define DEFINE_VISIT(Name) \
    virtual bool Visit##Name(Name* component) = 0;
    FOR_EACH_GUI_COMPONENT_TYPE(DEFINE_VISIT)
#undef DEFINE_VISIT
  };

  class Component {
    friend class Window;
    friend class OnMouseExitEvent;
    friend class OnMouseEnterEvent;
    friend class OnMouseClickEvent;
  protected:
    Component() = default;
    virtual void Publish(GuiEvent* event) = 0;

    template<class E, typename... Args>
    inline void Publish(Args... args) {
      E event(args...);
      return Publish((GuiEvent*) &event);
    }
  public:
    virtual ~Component() = default;
    virtual const Point& GetPos() const = 0;
    virtual const Size& GetSize() const = 0;
    virtual int32_t GetNumberOfChildren() const = 0;
    virtual Component* GetChildAt(const int32_t idx) const = 0;
    virtual bool Accept(ComponentVisitor* vis) = 0;
    virtual bool Contains(const Point& pos) const = 0;
    virtual void AddChild(Component* child) = 0;
    virtual rx::observable<GuiEvent*> OnEvent() const = 0;
    virtual const char* GetComponentName() const = 0;

    inline rx::observable<MouseEnterEvent*> OnMouseEnter() const {
      return OnEvent()
        .filter(MouseEnterEvent::Filter)
        .map(MouseEnterEvent::Cast);
    }

    inline rx::observable<MouseExitEvent*> OnMouseExit() const {
      return OnEvent()
        .filter(MouseExitEvent::Filter)
        .map(MouseExitEvent::Cast);
    }

    inline bool HasChildren() const {
      return GetNumberOfChildren() > 0;
    }

    inline bool IsLeaf() const {
      return GetNumberOfChildren() == 0;
    }
  };

  class ComponentBase : public Component {
  protected:
    rx::subject<GuiEvent*> events_;
    Point pos_;
    Size size_;

    ComponentBase();

    void SetPos(const Point& rhs) {
      pos_ = rhs;
    }

    void SetSize(const Size& rhs) {
      size_ = rhs;
    }

    void Publish(GuiEvent* event) override {
      MCC_ASSERT(event);
      const auto& subscriber = events_.get_subscriber();
      subscriber.on_next(event);
    }
  public:
    ~ComponentBase() override = default;

    const Point& GetPos() const override {
      return pos_;
    }

    const Size& GetSize() const override {
      return size_;
    }

    bool Contains(const Point& pos) const override {
      const auto& minX = pos_[0];
      const auto& maxX = pos_[0] + size_[0];
      const auto& minY = pos_[1];
      const auto& maxY = pos_[1] + size_[1];
      return pos[0] >= minX
          && pos[0] <= maxX
          && pos[1] >= minY
          && pos[1] <= maxY;
    }

    int32_t GetNumberOfChildren() const override {
      return 0;
    }

    Component* GetChildAt(const int32_t idx) const override {
      return nullptr;
    }

    void AddChild(Component* child) override {
      // do nothing
    }

    rx::observable<GuiEvent*> OnEvent() const override {
      return events_.get_observable();
    }
  };

#define DECLARE_GUI_COMPONENT_TYPE(Name)                                \
  public:                                                               \
    const char* GetComponentName() const override { return #Name; }

  class ContainerComponent : public ComponentBase {
  protected:
    std::vector<Component*> children_;

    ContainerComponent() = default;
  public:
    ~ContainerComponent() override = default;

    int32_t GetNumberOfChildren() const override {
      return children_.size();
    }

    Component* GetChildAt(const int32_t idx) const override {
      MCC_ASSERT(idx >= 0 && idx <= GetNumberOfChildren());
      return children_[idx];
    }

    void AddChild(Component* child) override {
      children_.push_back(child);
    }
  };
}

#endif //MCC_GUI_COMPONENT_H