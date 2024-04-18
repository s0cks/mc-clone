#ifndef MCC_COMPONENT_STATE_H
#define MCC_COMPONENT_STATE_H

#include <set>
#include <functional>
#include "mcc/entity/entity_id.h"

namespace mcc::component {
  class ComponentStateBase {
  public:
    struct EntityIdComparator {
      bool operator()(const ComponentStateBase* lhs, const ComponentStateBase* rhs) const {
        return lhs->entity() == rhs->entity();
      }
    };
  protected:
    EntityId entity_;

    explicit ComponentStateBase(const EntityId entity):
      entity_(entity) {
    }
  public:
    virtual ~ComponentStateBase() = default;
    virtual uword data() const = 0;

    EntityId entity() const {
      return entity_;
    }
  };

  template<class S>
  class ComponentState : public ComponentStateBase {
  protected:
    uword data_;
  public:
    ComponentState(const EntityId id, const uword data):
      ComponentStateBase(id),
      data_(data) {
    }
    ~ComponentState() override = default;

    uword data() const override {
      return data_;
    }

    S* Get() const {
      return (S*) data();
    }

    S* operator->() const {
      return Get();
    }
  };

  template<class S>
  class ComponentStateTable {
    typedef ComponentState<S> State;
    typedef std::function<State*> StateSupplier;
  private:
    class ComponentStateKey : public ComponentStateBase {
    public:
      explicit ComponentStateKey(const EntityId id):
        ComponentStateBase(id) {
      }
      ~ComponentStateKey() override = default;

      virtual uword data() const override {
        return 0;
      }
    };
  protected:
    std::set<ComponentStateBase*, ComponentStateBase::EntityIdComparator> states_;

    ComponentStateBase* GetState(const EntityId id) const {
      for(const auto state : states_) {
        if(state->entity() == id)
          return state;
      }
      return nullptr;
    }

    inline ComponentStateBase* PutState(ComponentStateBase* state) {
      const auto status = states_.insert(state);
      LOG_IF(ERROR, !status.second) << "failed to insert ComponentState into ComponentStateTable.";
      return state;
    }
  public:
    ComponentStateTable() = default;
    virtual ~ComponentStateTable() = default;

    virtual void Remove(const EntityId id) {
      NOT_IMPLEMENTED(FATAL);//TODO: implement
    }

    virtual bool Has(const EntityId id) const {
      return GetState(id) != nullptr;
    }

    virtual State* Create(const EntityId id, const uword data = 0) {
      return (State*) PutState(new State(id, data));
    }

    virtual State* GetOrCreate(const EntityId id) {
      const auto state = (State*) GetState(id);
      return state ? state : Create(id);
    }

    explicit operator rx::observable<State*> () const {
      return rx::observable<>::iterate(states_);
    }
  };
}

#endif //MCC_COMPONENT_STATE_H