#ifndef MCC_MOUSE_TRIE_H
#define MCC_MOUSE_TRIE_H

#include "mcc/trie.h"
#include "mcc/mouse/mouse.h"

namespace mcc {
  class SubscriptionNode : public trie::SequenceItemNode {
  protected:
    Mouse::Subscription subscription_;

    explicit SubscriptionNode(const Mouse::Subscription& subscription):
      trie::SequenceItemNode(),
      subscription_(subscription) {
    }
  public:
    ~SubscriptionNode() override = default;

    Mouse::Subscription GetSubscription() const {
      return subscription_;
    }

    friend bool operator==(const SubscriptionNode& lhs, const Mouse::Subscription& rhs) {
      return lhs.subscription_ == rhs;
    }

    friend bool operator!=(const SubscriptionNode& lhs, const Mouse::Subscription& rhs) {
      return lhs.subscription_ != rhs;
    }
  };

  template<class C>
  class SubscriptionNodeTemplate : public SubscriptionNode {
  protected:
    C callback_;
  public:
    SubscriptionNodeTemplate(const Mouse::Subscription& subscription, C callback):
      SubscriptionNode(subscription),
      callback_(callback) {
    }
    ~SubscriptionNodeTemplate() override = default;
  };

  template<typename C>
  class SubscriptionListNode : public trie::SequenceNode {
  protected:
    explicit SubscriptionListNode():
      trie::SequenceNode() {
    }
  public:
    ~SubscriptionListNode() override = default;

    void Register(const Mouse::Subscription& subscription, C callback) {
      const auto new_node = new SubscriptionNodeTemplate(subscription, callback);
      Append(new_node);
    }

    void Deregister(const Mouse::Subscription& subscription) {
      const auto head = (SubscriptionNode*)head_;
      if(head == nullptr)
        return;

      if((*head) == subscription) {
        head_ = head->GetNext();
        delete head_;
        return;
      }

      SequenceNodeIterator iter(this);
      while(iter.HasNext()) {
        const auto node = (SubscriptionNode*)iter.Next();
        if((*node) == subscription) {
          const auto prev = node->GetPrevious();
          const auto next = node->GetNext();
          if(prev)
            prev->SetNext(next);
          if(next)
            next->SetPrevious(prev);
          delete node;
          break;
        }
      }
    }
  };

  class MouseButtonStateNode;
  class MouseButtonStateSubscriptionNode : public SubscriptionNodeTemplate<MouseButtonCallback> {
  public:
    MouseButtonStateSubscriptionNode(const Mouse::Subscription& subscription, MouseButtonCallback callback):
      SubscriptionNodeTemplate(subscription, callback) {
    }
    ~MouseButtonStateSubscriptionNode() override = default;

    void Call(const Mouse::Subscription& subscription) {
      return callback_();
    }

    friend bool operator==(const MouseButtonStateSubscriptionNode& lhs, const Mouse::Subscription& rhs) {
      return lhs.subscription_ == rhs;
    }

    friend std::ostream& operator<<(std::ostream& stream, const MouseButtonStateSubscriptionNode& rhs) {
      stream << "MouseButtonStateSubscriptionNode(";
      stream << "subscription=" << rhs.subscription_;
      return stream << ")";
    }
  };

  class MouseButtonStateNode : public SubscriptionListNode<MouseButtonCallback> {
  protected:
    MouseButtonState state_;
  public:
    MouseButtonStateNode(const MouseButtonState state):
      SubscriptionListNode(),
      state_(state) {
    }
    ~MouseButtonStateNode() override = default;

    MouseButtonState GetState() const {
      return state_;
    }

    void Call(const Mouse::Subscription& subscription) {
      SequenceNodeIterator iter(this);
      while(iter.HasNext()) {
        const auto next = (MouseButtonStateSubscriptionNode*)iter.Next();
        next->Call(subscription);
      }
    }

    friend std::ostream& operator<<(std::ostream& stream, const MouseButtonStateNode& rhs) {
      stream << "MouseButtonStateNode(";
      stream << "state=" << rhs.GetState();
      return stream << ")";
    }
  };

  class MouseButtonNode : public trie::EdgeNode<kNumberOfMouseButtonStates> {
  protected:
    MouseButton button_;
  public:
    explicit MouseButtonNode(const MouseButton button):
      trie::EdgeNode<kNumberOfMouseButtonStates>(),
      button_(button) {
      for(auto idx = 0; idx < kNumberOfMouseButtonStates; idx++) {
        children_[idx] = new MouseButtonStateNode(static_cast<MouseButtonState>(idx));
      }
    }
    ~MouseButtonNode() override = default;

    MouseButton GetButton() const {
      return button_;
    }

    inline MouseButtonStateNode* GetButtonStateNode(const MouseButtonState state) const {
      return (MouseButtonStateNode*) children_[state];
    }

    inline MouseButtonStateNode* GetButtonStateNode(const Mouse::Subscription& subscription) const {
      return GetButtonStateNode(subscription.GetState());
    }

    void Call(const Mouse::Subscription& subscription) {
      return GetButtonStateNode(subscription)->Call(subscription);
    }

    void Register(const Mouse::Subscription& subscription, MouseButtonCallback callback) {
      return GetButtonStateNode(subscription)->Register(subscription, callback);
    }

    void Deregister(const Mouse::Subscription& subscription) {
      return GetButtonStateNode(subscription)->Deregister(subscription);
    }

    friend std::ostream& operator<<(std::ostream& stream, const MouseButtonNode& rhs) {
      stream << "MouseButtonNode(";
      stream << "button=" << rhs.GetButton();
      return stream << ")";
    }
  };

  class MouseButtonsNode : public trie::EdgeNode<kNumberOfMouseButtons> {
  public:
    explicit MouseButtonsNode():
      trie::EdgeNode<kNumberOfMouseButtons>() {
      for(auto idx = 0; idx < kNumberOfMouseButtons; idx++)
        children_[idx] = new MouseButtonNode(static_cast<MouseButton>(idx));
    }
    ~MouseButtonsNode() override = default;

    inline MouseButtonNode* GetMouseButtonNode(const MouseButton idx) const {
      return (MouseButtonNode*) children_[idx];
    }

    inline MouseButtonNode* GetMouseButtonNode(const Mouse::Subscription& subscription) const {
      return GetMouseButtonNode(subscription.GetButton());
    }

    void Call(const Mouse::Subscription& subscription) {
      return GetMouseButtonNode(subscription)->Call(subscription);
    }

    void Register(const Mouse::Subscription& subscription, MouseButtonCallback callback) {
      return GetMouseButtonNode(subscription)->Register(subscription, callback);
    }

    void Deregister(const Mouse::Subscription& subscription) {
      return GetMouseButtonNode(subscription)->Deregister(subscription);
    }
  };

  class MousePositionsNode;
  class MousePositionSubscriptionNode : public SubscriptionNodeTemplate<MousePositionCallback> {
  public:
    MousePositionSubscriptionNode(const Mouse::Subscription& subscription, MousePositionCallback callback):
      SubscriptionNodeTemplate(subscription, callback) {
    }
    ~MousePositionSubscriptionNode() override = default;

    void Call(const Mouse::Subscription& subscription, const glm::vec2 pos) {
      return callback_(pos);
    }

    friend bool operator==(const MousePositionSubscriptionNode& lhs, const Mouse::Subscription& rhs) {
      return lhs.subscription_ == rhs;
    }

    friend std::ostream& operator<<(std::ostream& stream, const MousePositionSubscriptionNode& rhs) {
      stream << "MouseButtonStateSubscriptionNode(";
      stream << "subscription=" << rhs.subscription_;
      return stream << ")";
    }
  };

  class MousePositionsNode : public SubscriptionListNode<MousePositionCallback> {
  public:
    explicit MousePositionsNode():
      SubscriptionListNode<MousePositionCallback>() {
    }
    ~MousePositionsNode() override = default;

    void Call(const Mouse::Subscription& subscription, const glm::vec2 pos) {
      SequenceNodeIterator iter(this);
      while(iter.HasNext()) {
        const auto next = (MousePositionSubscriptionNode*)iter.Next();
        next->Call(subscription, pos);
      }
    }
  };

  class RootNode : public trie::EdgeNode<2> {
  public:
    static constexpr const int kButtonsNodeIndex = 1;
    static constexpr const int kPositionNodeIndex = 0;
  public:
    RootNode():
      trie::EdgeNode<2>() {
      children_[kPositionNodeIndex] = new MousePositionsNode();
      children_[kButtonsNodeIndex] = new MouseButtonsNode();
    }
    ~RootNode() override = default;

    inline MouseButtonsNode* GetButtonsNode() const {
      return (MouseButtonsNode*) children_[kButtonsNodeIndex];
    }

    inline MousePositionsNode* GetPositionsNode() const {
      return (MousePositionsNode*) children_[kPositionNodeIndex];
    }

    void Call(const Mouse::Subscription& subscription) {
      return GetButtonsNode()->Call(subscription);
    }

    void Call(const Mouse::Subscription& subscription, const glm::vec2 pos) {
      return GetPositionsNode()->Call(subscription, pos);
    }

    void Register(const Mouse::Subscription& subscription, MouseButtonCallback callback) {
      return GetButtonsNode()->Register(subscription, callback);
    }

    void Register(const Mouse::Subscription& subscription, MousePositionCallback callback) {
      return GetPositionsNode()->Register(subscription, callback);
    }

    void Deregister(const Mouse::Subscription& subscription) {
      return subscription.IsPosition() 
          ? GetPositionsNode()->Deregister(subscription)
          : GetButtonsNode()->Deregister(subscription);
    }

    friend std::ostream& operator<<(std::ostream& stream, const RootNode& rhs) {
      stream << "RootNode()";
      return stream;
    }
  };
}

#endif //MCC_MOUSE_TRIE_H