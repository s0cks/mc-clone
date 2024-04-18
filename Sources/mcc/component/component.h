#ifndef MCC_COMPONENT_H
#define MCC_COMPONENT_H

#include <cstdint>
#include <unordered_map>
#include <array>
#include <typeinfo>

#include "mcc/gfx.h"
#include "mcc/relaxed_atomic.h"
#include "mcc/entity/entity.h"

#include "mcc/component/component_id.h"
#include "mcc/component/component_state.h"
#include "mcc/component/component_events.h"
#include "mcc/component/component_state_table.h"

namespace mcc {
  namespace component {
    class Component;
    
    class ComponentVisitor {
    protected:
      ComponentVisitor() = default;
    public:
      virtual ~ComponentVisitor() = default;
      virtual bool Visit(Component* component) = 0;
    };

    class Component {
      friend class Components;
    public:
      struct ComponentIdComparator {
        bool operator()(const Component* lhs, const Component* rhs) const {
          return lhs->GetComponentId() == rhs->GetComponentId();
        }
      };
    private:
      RelaxedAtomic<bool> registered_;
      RelaxedAtomic<ComponentId> id_;
      rx::subscription pre_init_sub_;

      inline void SetRegistered(const bool registered = true) {
        registered_ = registered;
      }

      inline void ClearRegistered() {
        return SetRegistered(false);
      }

      inline void SetComponentId(const ComponentId id) {
        id_ = id;
      }

      inline void ClearComponentId() {
        return SetComponentId(kInvalidComponentId);
      }
    protected:
      Component();
    public:
      virtual ~Component();
      virtual const char* GetName() const = 0;

      inline ComponentId GetComponentId() const {
        return (ComponentId) id_;
      }

      inline bool IsRegistered() const {
        return (bool) registered_;
      }
    };

    template<class State, const uint64_t NumberOfBuckets = kDefaultNumberOfComponentStateTableBuckets>
    class StatefulComponent : public Component {
    private:
      ComponentStateTable<State, NumberOfBuckets> states_;
    protected:
      StatefulComponent() = default;
    public:
      ~StatefulComponent() override = default;

      std::optional<ComponentState<State>> GetState(const Entity& e) const {
        return states_.Get(e);
      }
      
      bool Visit(std::function<bool(const Entity& e, const ComponentState<State>&)> vis) const {
        return states_.Visit(vis);
      }

      bool PutState(const Entity& e, const ComponentState<State>& state) {
        return states_.Put(e, state);
      }

      bool RemoveState(const Entity& e) {
        return states_.Remove(e);
      }
    };

    class Components {
      DEFINE_NON_INSTANTIABLE_TYPE(Components);
    public:
      static void Register(Component* component);
      static void ClearRegisteredComponents();
      static bool Visit(ComponentVisitor* vis);
      static rx::observable<Component*> Get();
      static rx::observable<ComponentEvent*> OnEvent();

      static inline rx::observable<ComponentRegisteredEvent*>
      OnRegistered() {
        return OnEvent()
          .filter(ComponentRegisteredEvent::Filter)
          .map(ComponentRegisteredEvent::Cast);
      }
    };
  }

  using component::Components;
  using component::Component;
  using component::StatefulComponent;

#define DECLARE_COMPONENT(Name)                               \
  public:                                                     \
    const char* GetName() const override { return #Name; }
}

#endif //MCC_COMPONENT_H