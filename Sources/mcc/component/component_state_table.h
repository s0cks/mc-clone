#ifndef MCC_COMPONENT_STATE_TABLE_H
#define MCC_COMPONENT_STATE_TABLE_H

#include "mcc/ecs/entity.h"
#include "mcc/component/component.h"

namespace mcc::component {
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

namespace mcc {
  using mcc::component::ComponentStateTable; //TODO: remove
}

#endif //MCC_COMPONENT_STATE_TABLE_H