#include <set>

#include "mcc/engine/engine.h"
#include "mcc/component/component.h"

namespace mcc::component {
  static rx::subject<ComponentEvent*> events_;
  static std::set<Component*, Component::ComponentIdComparator> components_;
  static RelaxedAtomic<ComponentId> next_id_;

  template<class E, typename... Args>
  static inline void
  Publish(Args... args) {
    E event(args...);
    return events_.get_subscriber().on_next(&event);
  }

  void Components::ClearRegisteredComponents() {
    components_.clear();
    next_id_ = 0;
  }

  void Components::Register(Component* component) {
    MCC_ASSERT(component);
    MCC_ASSERT(!component->IsRegistered());
    const auto name = component->GetName();
    const auto id = next_id_.fetch_add(1);
    component->SetComponentId(id);
    components_.insert(component);
    Publish<ComponentRegisteredEvent>(component);
    DLOG(INFO) << "component #" << id << " registered to: " << name;
  }

  bool Components::Visit(ComponentVisitor* vis) {
    for(const auto& component : components_) {
      if(!vis->Visit(component))
        return false;
    }
    return true;
  }

  rx::observable<Component*> Components::Get() {
    return rx::observable<>::iterate(components_);
  }

  rx::observable<ComponentEvent*> Components::OnEvent() {
    return events_.get_observable();
  }

  Component::Component():
    registered_(false),
    id_(kInvalidComponentId),
    pre_init_sub_() {
    const auto engine = engine::Engine::GetEngine();
    pre_init_sub_ = engine->OnPreInitEvent()
      .subscribe([this](engine::PreInitEvent* event) {
        Components::Register(this);
      });
  }

  Component::~Component() {
    //TODO: de-register component
    pre_init_sub_.unsubscribe();
  }
}