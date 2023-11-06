#ifndef MCC_COMPONENT_STATE_H
#define MCC_COMPONENT_STATE_H

#include <optional>
#include "mcc/platform.h"
#include "mcc/relaxed_atomic.h"
#include "mcc/ecs/entity.h"

namespace mcc {
  template<typename C>
  class ComponentState {
  private:
    RelaxedAtomic<uword> data_;
  public:
    ComponentState() = default;
    ComponentState(const uword data):
      data_(data) {
    }
    ComponentState(C* data):
      ComponentState((uword) data) {
    }
    ComponentState(const C& data):
      ComponentState(new C(data)) {
    }
    ComponentState(const ComponentState& rhs):
      data_(rhs.data_) {
    }
    ~ComponentState() = default;

    uword raw_ptr() const {
      return (uword) data_;
    }

    C* data() const {
      return (C*) raw_ptr();
    }

    C* operator->() const {
      return data();
    }

    explicit operator uword () const {
      return raw_ptr();
    }

    explicit operator C*() const {
      return data();
    }

    const C& operator *() const {
      return (*data());
    }

    void operator=(const ComponentState<C>& rhs) {
      data_ = rhs.data_;
    }

    void operator=(C* rhs) {
      data_ = (uword) rhs;
    }

    friend std::ostream& operator<<(std::ostream& stream, const ComponentState<C>& rhs) {
      stream << "ComponentState(";
      stream << "data=" << std::hex << rhs.data_;
      stream << ")";
      return stream;
    }
  };
  
  static constexpr const uint64_t kDefaultNumberOfComponentStateTableBuckets = 10;
  template<typename Component, const uint64_t NumberOfBuckets = kDefaultNumberOfComponentStateTableBuckets>
  class ComponentStateTable {
  private:
    struct Node {
      Node* next;
      Node* previous;
      Entity entity;
      ComponentState<Component> state;

      Node(const Entity& e, ComponentState<Component> s):
        next(nullptr),
        previous(nullptr),
        entity(e),
        state(s) {
      }
    };
  private:
    Node* nodes_[NumberOfBuckets];

    static inline constexpr uint64_t
    Hash(const Entity& e) {
      return static_cast<uint64_t>(e);
    }

    static inline constexpr uint64_t
    GetIndex(const Entity& e) {
      return Hash(e) % NumberOfBuckets;
    }

    static inline const char*
    GetComponentTypeName() {
      return typeid(Component).name();
    }
  public:
    ComponentStateTable():
      nodes_() {
      for(auto idx = 0; idx < NumberOfBuckets; idx++)
        nodes_[idx] = nullptr;
    }
    ComponentStateTable(const ComponentStateTable& rhs) = delete;
    virtual ~ComponentStateTable() = default;

    virtual std::optional<ComponentState<Component>> Get(const Entity& e) const {
      auto node = nodes_[GetIndex(e)];
      if(node == nullptr)
        return std::nullopt;

      do {
        if(node->entity == e)
          return std::optional<ComponentState<Component>>{node->state.data()};
        node = node->next;
      } while(node != nullptr);
      return std::nullopt;
    }

    virtual bool Remove(const Entity& e) {
      const auto index = GetIndex(e);
      auto node = nodes_[index];
      if(node == nullptr)
        return false;

      if(node->entity == e) {
        const auto next = node->next;
        if(next)
          next->previous = nullptr;
        nodes_[index] = next;
        delete node;
        return true;
      }

      while((node = node->next) != nullptr) {
        if(node->entity == e) {
          const auto next = node->next;
          const auto previous = node->previous;
          if(next)
            next->previous = previous;
          if(previous)
            previous->next = next;
          delete node;
          return true;
        }
      }
      return false;
    }

    virtual bool Put(const Entity& e, const ComponentState<Component>& value) {
      const auto index = GetIndex(e);
      auto node = nodes_[index];

      if(node == nullptr) {
        node = new Node(e, value);
        nodes_[index] = node;
        return true;
      }

      while(node != nullptr && node->entity < e)
        node = node->next;

      if(node->entity == e)
        return false;

      const auto next = node->next;
      const auto previous = node->previous;
      node = new Node(e, value);
      if(next)
        next->previous = node;
      if(previous)
        previous->next = node;
      return true;
    }

    virtual bool Visit(std::function<bool(const Entity&, const ComponentState<Component>&)> vis) const {
      for(auto bucket = 0; bucket < NumberOfBuckets; bucket++) {
        auto node = nodes_[bucket];
        while(node != nullptr) {
          if(!vis(node->entity, node->state))
            return false;
          node = node->next;
        }
      }
      return true;
    }

    void operator=(const ComponentStateTable& rhs) = delete;

    friend std::ostream& operator<<(std::ostream& stream, const ComponentStateTable<Component>& rhs) {
      stream << "ComponentStateTable<" << GetComponentTypeName() << ">(";
      stream << "num_buckets=" << NumberOfBuckets;
      stream << ")";
      return stream;
    }
  };
}

#endif //MCC_COMPONENT_STATE_H