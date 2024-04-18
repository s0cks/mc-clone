#ifndef MCC_COMPONENT_H
#define MCC_COMPONENT_H

#include <cstdint>
#include <set>
#include <unordered_map>
#include <array>
#include <typeinfo>

#include "mcc/gfx.h"
#include "mcc/relaxed_atomic.h"
#include "mcc/entity/entity.h"

#include "mcc/component/component_id.h"
#include "mcc/component/component_state.h"
#include "mcc/component/component_events.h"

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
      std::set<EntityId> entities_;
      Signature signature_;
      rx::subscription pre_init_sub_;
      rx::subscription entity_signature_changed_sub_;

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

      const Signature& GetSignature() const {
        return signature_;
      }

      inline ComponentId GetComponentId() const {
        return (ComponentId) id_;
      }

      inline bool IsRegistered() const {
        return (bool) registered_;
      }
    };

    template<class S>
    class StatefulComponent : public Component,
                              public EntityDestroyedListener,
                              public EntitySignatureChangedListener {
      typedef ComponentState<S> State;
      typedef rx::observable<State*> StateObservable;
      typedef std::function<S*()> StateSupplier;
    private:
      ComponentStateTable<S> states_;
    protected:
      StatefulComponent():
        Component(),
        EntityDestroyedListener(),
        EntitySignatureChangedListener(),
        states_() {
      }

      bool RemoveState(const EntityId id) {
        return states_.Remove(id);
      }

      void OnEntitySignatureChanged(EntitySignatureChangedEvent* event) override {
        NOT_IMPLEMENTED(ERROR); //TODO: implement
      }

      void OnEntityDestroyed(EntityDestroyedEvent* event) override {
        NOT_IMPLEMENTED(ERROR); //TODO: implement
      }
    public:
      ~StatefulComponent() override = default;

      bool HasState(const EntityId id) const {
        return states_.Has(id);
      }

      State* GetState(const EntityId id) const {
        return states_.Get(id);
      }

      State* GetOrCreateState(const EntityId id) {
        return states_.GetOrCreate(id);
      }

      State* CreateState(const EntityId id, const S* data = nullptr) {
        return states_.Create(id, (const uword) data);
      }

      StateObservable GetStates() const {
        return (StateObservable) states_;
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

#define DEFINE_ON_EVENT(Name)                                                 \
      static inline rx::observable<Name##Event*>                              \
      On##Name##Event() {                                                     \
        return OnEvent()                                                      \
          .filter(Name##Event::Filter)                                        \
          .map(Name##Event::Cast);                                            \
      }
      FOR_EACH_COMPONENT_EVENT(DEFINE_ON_EVENT)
#undef DEFINE_ON_EVENT

#ifdef MCC_DEBUG
      static void PrintAll(const google::LogSeverity severity = google::INFO);
#endif //MCC_DEBUG
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